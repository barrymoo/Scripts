CXX=g++
FLAGS=-Wall -O2 -std=c++11 -lrt -fopenmp
LIBRARY=-I/usr/include/eigen3 -I/usr/include
TARGET=grid-orbital-unitary-transformation
SOURCE=grid-orbital-unitary-transformation.cpp CubeFile.cpp

$(TARGET):$(SOURCE) 
	$(CXX) $(FLAGS) $(LIBRARY) $^ -o $@
