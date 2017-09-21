cmake_minimum_required(VERSION 3.2)
project(server)

set(CMAKE_CXX_STANDARD 14)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../common")

set(SOURCE_FILES main.cpp)
add_executable(server ${SOURCE_FILES})
target_link_libraries(server common)