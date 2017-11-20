cmake_minimum_required(VERSION 3.2)
project(libsts)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_FILES phy/IPhysicalLayer.h phy/PhysicalLayerException.h link/LinkLayer.cpp link/LinkLayer.h link/LinkLayerExceptions.h libsts.h util/crc-16-ansi.h phy/NetworkBasedPhysicalLayer.cpp phy/NetworkBasedPhysicalLayer.h link/ErrorCorrectionType.h util/hamming.h)
add_library(sts SHARED ${SOURCE_FILES})
