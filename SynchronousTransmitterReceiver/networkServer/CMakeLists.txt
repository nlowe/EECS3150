cmake_minimum_required(VERSION 3.2)
project(networkServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../common")

set(SOURCE_FILES main.cpp)
add_executable(networkServer ${SOURCE_FILES})
target_link_libraries(networkServer sts)
