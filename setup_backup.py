from distutils.core import setup, Extension
import numpy

module = Extension('pyyolo',
	library_dirs=['.'],
	libraries=['yolo'],
	include_dirs=[numpy.get_include(), './darknet/include'],
	sources = ['module.c'],
	extra_compile_args=['-fopenmp'],
	extra_link_args=['-lgomp', '-lm', '-lmvec'])

setup (name = 'pyyolo',
	version = '0.1',
	description = 'YOLO wrapper',
	ext_modules = [module])
