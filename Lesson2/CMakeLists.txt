cmake_minimum_required(VERSION 3.16)
project(Lesson2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lesson2 task5.cpp task6.cpp task7.cpp task8.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")