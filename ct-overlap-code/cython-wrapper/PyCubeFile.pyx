# distutils: language = c++
# distutils: include_dirs = /usr/include/python3.3m
# distutils: extra_compile_args = ["-std=c++11", "-fopenmp"]
# distutils: sources = CubeFile.cpp

from libcpp.string cimport string

cdef extern from "CubeFile.h":
    cppclass CubeFile:
        CubeFile()
        CubeFile(string)
        CubeFile(CubeFile)

cdef class PyCubeFile:
    cdef CubeFile *thisptr
    def __cinit__(self):
        self.thisptr = new CubeFile()
