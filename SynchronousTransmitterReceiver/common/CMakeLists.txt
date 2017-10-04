cmake_minimum_required(VERSION 3.2)
project(libsts)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES phy/IPhysicalLayer.h phy/FileBasedPhysicalLayer.cpp phy/FileBasedPhysicalLayer.h phy/PhysicalLayerException.h link/LinkLayer.cpp link/LinkLayer.h link/LinkLayerExceptions.h)
add_library(sts SHARED ${SOURCE_FILES})
