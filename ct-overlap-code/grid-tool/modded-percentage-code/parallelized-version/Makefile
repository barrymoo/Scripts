CXX=g++
FLAGS=-Wall -O3 -std=c++11 -fopenmp
LIBRARY=-I/usr/include/eigen3 -I/usr/include
TARGET=grid-orbital-unitary-transformation
SOURCE=grid-orbital-unitary-transformation.cpp CubeFile.cpp

$(TARGET):$(SOURCE) 
	$(CXX) $(FLAGS) $(LIBRARY) $^ -o $@
