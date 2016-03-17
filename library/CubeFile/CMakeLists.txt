cmake_minimum_required(VERSION 3.4)

project(CubeFile)

# Find Python
find_package(PythonLibs REQUIRED)

# C++11
set(CMAKE_CXX_STANDARD 11)

if(APPLE)
    SET(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
endif()

# Include Directory for pybind11
include_directories(${PYTHON_INCLUDE_DIRS} /Users/bmooreii/src/pybind11/include)

# SOURCES and SHARED Library
set(SOURCES CubeFile-pybind11.cpp CubeFile.cpp)
add_library(CubeFile SHARED ${SOURCES})

#set_target_properties(CubeFile PROPERTIES PREFIX "")

if(APPLE)
    set_target_properties(CubeFile PROPERTIES MACOSX_RPATH ".")
    set_target_properties(CubeFile PROPERTIES LINK_FLAGS "-undefined dynamic_lookup ")
endif()
