from distutils.core import setup, Extension

module = Extension('pyyolo',
	library_dirs=['.', '/usr/local/cuda/lib64'],
	libraries=['yolo', 'cuda', 'cudart', 'cublas', 'curand', 'cudnn'],
	sources = ['module.c'])

setup (name = 'pyyolo',
	version = '0.1',
	description = 'YOLO wrapper',
	ext_modules = [module])
