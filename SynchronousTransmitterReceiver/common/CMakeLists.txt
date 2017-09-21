cmake_minimum_required(VERSION 3.2)
project(common)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES library.cpp library.h)
add_library(common SHARED ${SOURCE_FILES})
