cmake_minimum_required(VERSION 3.16)
project(Method)

set(CMAKE_CXX_STANDARD 14)

add_executable(Method method.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")