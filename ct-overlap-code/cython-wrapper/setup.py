from distutils.core import setup
from Cython.Build import cythonize

setup(name = "PyCubeFile", ext_modules = cythonize("PyCubeFile.pyx"))
