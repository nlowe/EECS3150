cmake_minimum_required(VERSION 3.2)
project(libsts)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES library.cpp library.h phy/IPhysicalLayer.h phy/FileBasedPhysicalLayer.cpp phy/FileBasedPhysicalLayer.h phy/PhysicalLayerException.h link/LinkLayer.cpp link/LinkLayer.h link/Frame.h)
add_library(sts SHARED ${SOURCE_FILES})
