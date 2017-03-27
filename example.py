import pyyolo
import sys
import cv2

datacfg = 'cfg/coco.data'
cfgfile = 'cfg/tiny-yolo.cfg'
weightfile = '/home/tiny-yolo.weights'
thresh = 0.24
hier_thresh = 0.5

pyyolo.init(datacfg, cfgfile, weightfile)

cam = cv2.VideoCapture(0)
while True:
	ret_val, img = cam.read()
		# if cv2.waitKey(1) == 27: 
		#	break  # esc to quit
		# cv2.destroyAllWindows()
	src = cv.CreateImageHeader((img.shape[1], img.shape[0]), cv.IPL_DEPTH_8U, 3)
	cv.SetData(bitmap, img.tostring(), img.dtype.itemsize * 3 * img.shape[1])	
	outputs = pyyolo.test(src, thresh, hier_thresh)	

	for output in outputs:
		print(output)
	
pyyolo.cleanup()