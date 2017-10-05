cmake_minimum_required(VERSION 3.2)
project(faulty-wire)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES main.cpp)
add_executable(faulty-wire ${SOURCE_FILES})