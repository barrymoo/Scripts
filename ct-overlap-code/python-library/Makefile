CXX=g++
FLAGS=-Wall -O2 -std=c++11 -fopenmp -shared -fPIC
LIBRARY=-I/usr/include/boost -I/usr/include/python3.3m -lpython3.3m -lboost_python3
TARGET=CubeFile.so
SOURCE=CubeFile-python.cpp CubeFile.cpp

$(TARGET):$(SOURCE) 
	$(CXX) $(FLAGS) $(LIBRARY) $^ -o $@
