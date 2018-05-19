#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>
#include <stdio.h>
#include "libyolo.h"
#include "./darknet/src/image.h"

static PyObject *PyyoloError;
static yolo_handle g_handle = NULL;

static PyObject *pyyolo_init(PyObject *self, PyObject *args)
{
	char *darknet_path;
	char *datacfg;
	char *cfgfile;
	char *weightfile;

	if (!PyArg_ParseTuple(args, "ssss", &darknet_path, &datacfg, &cfgfile, &weightfile))
		return NULL;

	g_handle = yolo_init(darknet_path, datacfg, cfgfile, weightfile);
	if (!g_handle) {
		PyErr_SetString(PyyoloError, "Initialzing YOLO failed");
		return Py_None;
	}

	return Py_None;
}

static PyObject *pyyolo_cleanup(PyObject *self, PyObject *args)
{
	yolo_cleanup(g_handle);
	g_handle = NULL;

	return Py_None;
}

static PyObject *pyyolo_detect(PyObject *self, PyObject *args)
{
	int w;
	int h;
	int c;
	PyArrayObject *array;
	float thresh;
	float hier_thresh;
	
	if (!PyArg_ParseTuple(args, "iiiOff", &w, &h, &c, &array, &thresh, &hier_thresh))
		return NULL;

	// convert (copy) PyArrayObject(float32) to float []
	// option 1

	// float data[w*h*c];
	// int k;
	// for (k = 0; k < w*h*c; k++) {
	//	data[k] = *((float*) PyArray_GETPTR1(array, k));
	// }

	// option 2
	float *data;
	data = (float*) PyArray_GETPTR1(array, 0);
	
    // The below should work, data is a ptr to a c array
	image img = float_to_image(w, h, c, data);
	rgbgr_image(img);
	int num = 0;
	detection_info **info = yolo_detect(g_handle, img, thresh, hier_thresh, &num);
	if (info == NULL) {
		PyErr_SetString(PyyoloError, "Testing YOLO failed");
		return Py_None;
	}
	
	PyObject *dict = NULL;
	PyObject *list = PyList_New(num);
	int i;

	for (i = 0; i < num; i++) {
		dict = Py_BuildValue("{s:s,s:i,s:i,s:i,s:i,s:f}",
			"class", info[i]->name,
			"left", info[i]->left,
			"right", info[i]->right,
			"top", info[i]->top,
			"bottom", info[i]->bottom,
			"prob", info[i]->prob);
		PyList_SetItem(list, i, dict);
		free(info[i]);
	}
	free(info);
	
	return list;
}

static PyObject *pyyolo_test(PyObject *self, PyObject *args)
{
	char *filename;
	float thresh;
	float hier_thresh;
	float *feature_map = NULL;
	int map_size = 0;
	int return_map = 0;

	if (!PyArg_ParseTuple(args, "sffi", &filename, &thresh, &hier_thresh, &return_map))
		return NULL;

	int num = 0;
	detection_info **info = yolo_test(g_handle, filename, thresh, hier_thresh, &num, &feature_map, &map_size);
	if (info == NULL) {
		PyErr_SetString(PyyoloError, "Testing YOLO failed");
		return Py_None;
	}

	PyObject *dict = NULL;
	PyObject *list = NULL;

	if (return_map) {
		list = PyList_New(num + 1);
	} else {
		list = PyList_New(num);
	}

	int i;
	for (i = 0; i < num; i++) {
		dict = Py_BuildValue("{s:s,s:i,s:i,s:i,s:i,s:f}",
			"class", info[i]->name,
			"left", info[i]->left,
			"right", info[i]->right,
			"top", info[i]->top,
			"bottom", info[i]->bottom,
			"prob", info[i]->prob);
		PyList_SetItem(list, i, dict);
		free(info[i]);
	}
	free(info);

	/* if (return_map) {
		import_array();
		npy_intp dims[] = {map_size};
		PyObject *data = PyArray_SimpleNewFromData(1, dims, NPY_FLOAT, feature_map);
		Py_INCREF(data);
		dict = Py_BuildValue("{s:O}", "feature_map", data);
		PyList_SetItem(list, i, dict);
	} */

	return list;
}

static PyMethodDef pyyolo_methods[] = {
	{"init",  pyyolo_init, METH_VARARGS, "Initialize YOLO."},
	{"cleanup",  pyyolo_cleanup, METH_VARARGS, "Cleanup YOLO."},
	{"detect",  pyyolo_detect, METH_VARARGS, "Test image."},
	{"test",  pyyolo_test, METH_VARARGS, "Test image."},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

#if PY_MAJOR_VERSION >= 3
static struct PyModuleDef pyyolo_module =
{
        PyModuleDef_HEAD_INIT,
        "pyyolo",
        "",
        -1,
        pyyolo_methods
};

PyMODINIT_FUNC PyInit_pyyolo(void)
{
	PyObject *m;

	m = PyModule_Create(&pyyolo_module);
	if (m == NULL) return;

	PyyoloError = PyErr_NewException("pyyolo.error", NULL, NULL);
	Py_INCREF(PyyoloError);
	PyModule_AddObject(m, "error", PyyoloError);

	return m;
}
#else
PyMODINIT_FUNC initpyyolo(void)
{
	PyObject *m;

	m = Py_InitModule("pyyolo", pyyolo_methods);
	if (m == NULL) return;

	PyyoloError = PyErr_NewException("pyyolo.error", NULL, NULL);
	Py_INCREF(PyyoloError);
	PyModule_AddObject(m, "error", PyyoloError);
}
#endif
