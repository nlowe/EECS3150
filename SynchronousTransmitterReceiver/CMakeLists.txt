cmake_minimum_required(VERSION 3.2)
project(SynchronousTransmitterReceiver)

add_subdirectory(common)
add_subdirectory(networkClient)
add_subdirectory(networkServer)

#need to rewrite tests for network-based-phy
#add_subdirectory(tests)
