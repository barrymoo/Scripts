#!/bin/bash

cython --cplus CubeFile.pyx -o CubeFile-cython.cpp
g++ -shared -fopenmp -fPIC CubeFile-cython.cpp -I/usr/include/python3.3m -std=c++11 -o CubeFile.so
