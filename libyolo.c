#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "darknet.h"
#include "option_list.h"
#include "network.h"
#include "parser.h"
#include "region_layer.h"
#include "utils.h"
#include "libyolo.h"

typedef struct {
	char darknet_path[1024];
	char **names;
	float nms;
	box *boxes;
	float **probs;
	network *net;
} yolo_obj;

void get_detection_info(image im, int num, float thresh, box *boxes, float **probs, int classes, char **names, list *output)
{
	int i;

	for(i = 0; i < num; ++i){
		int class = max_index(probs[i], classes);
		float prob = probs[i][class];
		if(prob > thresh){
			box b = boxes[i];

			int left  = (b.x-b.w/2.)*im.w;
			int right = (b.x+b.w/2.)*im.w;
			int top   = (b.y-b.h/2.)*im.h;
			int bot   = (b.y+b.h/2.)*im.h;

			if(left < 0) left = 0;
			if(right > im.w-1) right = im.w-1;
			if(top < 0) top = 0;
			if(bot > im.h-1) bot = im.h-1;

			detection_info *info = (detection_info *)malloc(sizeof(detection_info));
			strncpy(info->name, names[class], sizeof(info->name));
			info->left = left;
			info->right = right;
			info->top = top;
			info->bottom = bot;
			info->prob = prob;
			list_insert(output, info);
		}
	}
}

yolo_handle yolo_init(char *darknet_path, char *datacfg, char *cfgfile, char *weightfile)
{
	yolo_obj *obj = (yolo_obj *)malloc(sizeof(yolo_obj));
	if (!obj) return NULL;
	memset(obj, 0, sizeof(yolo_obj));

	char cur_dir[1024];
	strncpy(obj->darknet_path, darknet_path, sizeof(obj->darknet_path));
	getcwd(cur_dir, sizeof(cur_dir));
	chdir(darknet_path);
	list *options = read_data_cfg(datacfg);
	char *name_list = option_find_str(options, "names", "data/names.list");
	obj->names = get_labels(name_list);

	obj->net = load_network(cfgfile, weightfile, 0);
	printf("Load netork. \n");
	set_batch_network(obj->net, 1);
	printf("Set batch network. \n");

	srand(2222222);

	int j;
	obj->nms=.4;

	layer l = obj->net->layers[obj->net->n-1];
	obj->boxes = calloc(l.w*l.h*l.n, sizeof(box));
	obj->probs = calloc(l.w*l.h*l.n, sizeof(float *));
	for(j = 0; j < l.w*l.h*l.n; ++j) obj->probs[j] = calloc(l.classes + 1, sizeof(float *));
	chdir(cur_dir);

	return (yolo_handle)obj;
}

void yolo_cleanup(yolo_handle handle)
{
	yolo_obj *obj = (yolo_obj *)handle;
	if (obj) {
		layer l = obj->net->layers[obj->net->n-1];
		free(obj->boxes);
		free_ptrs((void **)obj->probs, l.w*l.h*l.n);
		free(obj);
	}
}

detection_info **yolo_detect(yolo_handle handle, image im, float thresh, float hier_thresh, int *num)
{
	yolo_obj *obj = (yolo_obj *)handle;
	image sized = letterbox_image(im, obj->net->w, obj->net->h);

	float *X = sized.data;
	clock_t time;
	time=clock();
	network_predict(obj->net, X);
	printf("Cam frame predicted in %f seconds.\n", sec(clock()-time));

	layer l = obj->net->layers[obj->net->n-1];
	get_region_boxes(l, im.w, im.h, obj->net->w, obj->net->h, thresh, obj->probs, obj->boxes, NULL, 0, 0, hier_thresh, 1);
	if (obj->nms) do_nms_obj(obj->boxes, obj->probs, l.w*l.h*l.n, l.classes, obj->nms);

	list *output = make_list();
	get_detection_info(im, l.w*l.h*l.n, thresh, obj->boxes, obj->probs, l.classes, obj->names, output);
	detection_info **info = (detection_info **)list_to_array(output);
	*num = output->size;

	free_list(output);
	// free_image(im);
	free_image(sized);

	return info;
}

detection_info **yolo_test(yolo_handle handle, char *filename, float thresh, float hier_thresh, int *num, float **feature_map, int *map_size)
{
	yolo_obj *obj = (yolo_obj *)handle;

	char input[256];
	strncpy(input, filename, sizeof(input));

	image im = load_image_color(input,0,0);
	image sized = letterbox_image(im, obj->net->w, obj->net->h);

	float *X = sized.data;
	clock_t time;
	time=clock();
	network_predict(obj->net, X);
	*feature_map = obj->net->output;
	*map_size = obj->net->outputs;
	printf("%s: Predicted in %f seconds.\n", input, sec(clock()-time));

	layer l = obj->net->layers[obj->net->n-1];
	get_region_boxes(l, im.w, im.h, obj->net->w, obj->net->h, thresh, obj->probs, obj->boxes, NULL, 0, 0, hier_thresh, 1);
	if (obj->nms) do_nms_obj(obj->boxes, obj->probs, l.w*l.h*l.n, l.classes, obj->nms);

	list *output = make_list();
	get_detection_info(im, l.w*l.h*l.n, thresh, obj->boxes, obj->probs, l.classes, obj->names, output);
	detection_info **info = (detection_info **)list_to_array(output);
	*num = output->size;

	free_list(output);
	free_image(im);
	free_image(sized);

	return info;
}
