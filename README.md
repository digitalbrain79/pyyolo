# pyyolo
pyyolo is a simple wrapper for YOLO.

## Building
1. git clone https://github.com/thomaspark-pkj/pyyolo.git
2. In pyyolo directory, git clone https://github.com/pjreddie/darknet.git
3. Edit Makefile to use GPU.
4. make
5. python setup.py build
6. sudo python setup.py install

## Test
Edit parameters in example.py
```bash
python example.py
```
Result
```bash
{'bottom': 338, 'top': 109, 'right': 594, 'class': 'horse', 'left': 411}
{'bottom': 381, 'top': 101, 'right': 274, 'class': 'person', 'left': 184}
{'bottom': 347, 'top': 137, 'right': 583, 'class': 'sheep', 'left': 387}
{'bottom': 353, 'top': 264, 'right': 194, 'class': 'dog', 'left': 71}
```
