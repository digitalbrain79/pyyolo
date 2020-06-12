from distutils.core import setup, Extension
import numpy

module = Extension('pyyolo',
	library_dirs=['.', '/usr/local/cuda/lib64', '/usr/local/'],
	libraries=['yolo', 'cuda', 'cudart', 'cublas', 'curand', 'cudnn'],
    extra_compile_args=['-fopenmp'],
    extra_link_args=['-lgomp', '-lm', '-lmvec'],
	include_dirs=[numpy.get_include(), './darknet/include'],
	sources = ['module.c'])

setup (name = 'pyyolo',
	version = '0.1',
	description = 'YOLO wrapper',
	ext_modules = [module])
