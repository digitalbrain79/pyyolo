#include <Python.h>
#include "libyolo.h"

static PyObject *PyyoloError;
static yolo_handle g_handle = NULL;

static PyObject *pyyolo_init(PyObject *self, PyObject *args)
{
	char *datacfg;
	char *cfgfile;
	char *weightfile;

	if (!PyArg_ParseTuple(args, "sss", &datacfg, &cfgfile, &weightfile))
		return NULL;

	g_handle = yolo_init(datacfg, cfgfile, weightfile);
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

static PyObject *pyyolo_test(PyObject *self, PyObject *args)
{
	IplImage *src;
	float thresh;
	float hier_thresh;

	if (!PyArg_ParseTuple(args, "sff", &src, &thresh, &hier_thresh))
		return NULL;

	int num = 0;
	detection_info **info = yolo_test(g_handle, src, thresh, hier_thresh, &num);
	if (info == NULL) {
		PyErr_SetString(PyyoloError, "Testing YOLO failed");
		return Py_None;
	}

	PyObject *dict = NULL;
	PyObject *list = PyList_New(num);
	int i;

	for (i = 0; i < num; i++) {
		dict = Py_BuildValue("{s:s,s:i,s:i,s:i,s:i}",
			"class", info[i]->name,
			"left", info[i]->left,
			"right", info[i]->right,
			"top", info[i]->top,
			"bottom", info[i]->bottom);
		PyList_SetItem(list, i, dict);
		free(info[i]);
	}
	free(info);

	return list;
}

static PyMethodDef PyyoloMethods[] = {
	{"init",  pyyolo_init, METH_VARARGS, "Initialize YOLO."},
	{"cleanup",  pyyolo_cleanup, METH_VARARGS, "Cleanup YOLO."},
	{"test",  pyyolo_test, METH_VARARGS, "Test image."},
	{NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpyyolo(void)
{
	PyObject *m;

	m = Py_InitModule("pyyolo", PyyoloMethods);
	if (m == NULL) return;

	PyyoloError = PyErr_NewException("pyyolo.error", NULL, NULL);
	Py_INCREF(PyyoloError);
	PyModule_AddObject(m, "error", PyyoloError);
}
