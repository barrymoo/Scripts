CXX=g++
FLAGS=-Wall -O3 -std=c++11 -shared -fPIC -fopenmp
LIBRARY=-I/usr/lib/boost -I/usr/include/python3.4m -lpython3.4m -lboost_python3
TARGET=CubeFile.so
SOURCE=CubeFile-python.cpp CubeFile.cpp

$(TARGET):$(SOURCE) 
	$(CXX) $(FLAGS) $(LIBRARY) $^ -o $@
