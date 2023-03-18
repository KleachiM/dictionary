cmake_minimum_required(VERSION 3.24)
project(dictionary)

set(CMAKE_CXX_STANDARD 17)

add_executable(dictionary main.cpp Functions.cpp Functions.h)
