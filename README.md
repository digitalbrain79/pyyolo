# pyyolo
pyyolo is a simple wrapper for YOLO (compatible with YOLOv3).

## Prerequisites
Install open-cv:
[https://docs.opencv.org/3.4.1/d2/de6/tutorial_py_setup_in_ubuntu.html](https://docs.opencv.org/3.4.1/d2/de6/tutorial_py_setup_in_ubuntu.html)

To use opencv with Python 3 install opencv with (should also install numpy):
pip3 install opencv-python

## Building
1. Clone this repo
2. (optional) Set GPU=1 and CUDNN=1 in Makefile to use GPU.
3. make
4. rm -rf build
5. python3 setup.py build (use setup_gpu.py for GPU) (setup_gpy.py has not been modified or tested to build properly)
6. sudo python3 setup.py install (use setup_gpu.py for GPU)

## Test
Edit parameters in example.py if needed
```bash
python3 example.py
```
Result
```bash
{'right': 194, 'bottom': 353, 'top': 264, 'class': 'dog', 'prob': 0.8198755383491516, 'left': 71}
{'right': 594, 'bottom': 338, 'top': 109, 'class': 'horse', 'prob': 0.6106302738189697, 'left': 411}
{'right': 274, 'bottom': 381, 'top': 101, 'class': 'person', 'prob': 0.702547550201416, 'left': 184}
{'right': 583, 'bottom': 347, 'top': 137, 'class': 'sheep', 'prob': 0.7186083197593689, 'left': 387}
```
