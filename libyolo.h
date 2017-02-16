#ifndef LIBYOLO_H
#define LIBYOLO_H

typedef void* yolo_handle;

typedef struct {
	char name[32];
	int left;
	int right;
	int top;
	int bottom;
} detection_info;

yolo_handle yolo_init(char *datacfg, char *cfgfile, char *weightfile);
void yolo_cleanup(yolo_handle handle);
detection_info **yolo_test(yolo_handle handle, char *filename, float thresh, float hier_thresh, int *num);

#endif // LIBYOLO_H
