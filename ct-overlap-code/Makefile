CXX=g++
LIBRARY=/usr/include/eigen3
TARGET=ct-overlap-tool
SOURCE=ct-overlap-tool.cpp CubeFile.cpp

$(TARGET):$(SOURCE) 
	$(CXX) -O2 -I$(LIBRARY) -std=c++11 $^ -o $@
