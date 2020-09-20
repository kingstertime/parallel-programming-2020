cmake_minimum_required(VERSION 3.16)
project(Lesson3)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lesson3 task9.cpp task10.cpp task11.cpp task12.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")