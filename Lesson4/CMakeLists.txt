cmake_minimum_required(VERSION 3.16)
project(Lesson4)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lesson4 task13.cpp task14.cpp task15.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
