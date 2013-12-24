CXX=g++
FLAGS=-Wall -O3 -std=c++11 -fopenmp
MYLIB=/home/barrymoo/barry-programming/scripts/library/CubeFile
LIBRARY=-I/usr/include/eigen3 -I/usr/include -I$(MYLIB)
TARGET=ut-overlap-tool
SOURCE=$(TARGET).cpp $(MYLIB)/CubeFile.cpp

$(TARGET):$(SOURCE) 
	$(CXX) $(FLAGS) $(LIBRARY) $^ -o $@
