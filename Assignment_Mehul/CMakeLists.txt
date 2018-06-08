cmake_minimum_required(VERSION 2.8.9)
project(test)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
#Bring the headers, such as main.h into the project
#include_directories(include)
include_directories(./)

#Can manually add the sources using the set command as follows:
set(SOURCES ./main.cpp)

#However, the file(GLOB...) allows for wildcard additions:
#file(GLOB SOURCES "src/*.cpp")

add_executable(test ${SOURCES})
