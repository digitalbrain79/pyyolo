import pyyolo
import sys
import cv2
from cv2 import cv

datacfg = 'cfg/coco.data'
cfgfile = 'cfg/tiny-yolo.cfg'
weightfile = '../tiny-yolo.weights'
thresh = 0.24
hier_thresh = 0.5

pyyolo.init(datacfg, cfgfile, weightfile)
pyyolo.fetch_frame()
# cam = cv2.VideoCapture(0)
# while True:
# 	ret_val, img = cam.read()
# 	src = cv.CreateImageHeader((img.shape[1], img.shape[0]), cv.IPL_DEPTH_8U, 3)
# 	cv.SetData(src, img.tostring(), img.dtype.itemsize * 3 * img.shape[1])	

outputs = pyyolo.test(src, thresh, hier_thresh)	
for output in outputs:
	print(output)
	
pyyolo.cleanup()