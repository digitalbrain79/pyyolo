import pyyolo
import sys

datacfg = 'cfg/coco.data'
cfgfile = 'cfg/tiny-yolo.cfg'
weightfile = '/home/thomas/Downloads/dl/weights/tiny-yolo.weights'
filename = 'data/person.jpg'
thresh = 0.24
hier_thresh = 0.5

pyyolo.init(datacfg, cfgfile, weightfile)
outputs = pyyolo.test(filename, thresh, hier_thresh)
for output in outputs:
    print(output)
pyyolo.cleanup()