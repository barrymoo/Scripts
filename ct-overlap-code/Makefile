CXX=g++
LIBRARY=/usr/include/eigen3
TARGET=ct-overlap-tool
SOURCE=ct-overlap-tool.cpp CubeFile.cpp

$(TARGET):$(SOURCE) 
	$(CXX) -I$(LIBRARY) -std=gnu++11 $^ -o $@
