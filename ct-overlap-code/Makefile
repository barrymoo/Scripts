CXX=g++
FLAGS=-Wall -O2 -std=c++11 -lrt -fopenmp
LIBRARY=-I/usr/include/eigen3
TARGET=ct-overlap-tool
SOURCE=ct-overlap-tool.cpp CubeFile.cpp

$(TARGET):$(SOURCE) 
	$(CXX) $(FLAGS) $(LIBRARY) $^ -o $@
