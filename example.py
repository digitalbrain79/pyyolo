import pyyolo
import numpy as np
import sys
import cv2

darknet_path = './darknet'
datacfg = 'cfg/coco.data'
cfgfile = 'cfg/tiny-yolo.cfg'
weightfile = '../tiny-yolo.weights'
filename = darknet_path + '/data/dog.jpg'
thresh = 0.3
hier_thresh = 0.5
# cam = cv2.VideoCapture(-1)
# ret_val, img = cam.read()
# print(ret_val)
# if ret_val:
#     ret_val = cv2.imwrite(filename,img)
#     print(ret_val)

pyyolo.init(darknet_path, datacfg, cfgfile, weightfile)

# from file
print('----- test original C using a file')
outputs = pyyolo.test(filename, thresh, hier_thresh, 0)
for output in outputs:
	print(output)

# camera 
# print('----- test python API using a file')
# i = 1
# while i < 2:
# 	# ret_val, img = cam.read()
# 	img = cv2.imread(filename)
# 	img = img.transpose(2,0,1)
# 	c, h, w = img.shape[0], img.shape[1], img.shape[2]
# 	# print w, h, c 
# 	data = img.ravel()/255.0
# 	data = np.ascontiguousarray(data, dtype=np.float32)
# 	outputs = pyyolo.detect(w, h, c, data, thresh, hier_thresh)	
# 	for output in outputs:
# 		print(output)
# 	i = i + 1


# free model
pyyolo.cleanup()
