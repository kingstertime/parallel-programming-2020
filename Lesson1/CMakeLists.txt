cmake_minimum_required(VERSION 3.16)
project(Lesson1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Lesson1 task1.cpp task2.cpp task3.cpp task4.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")