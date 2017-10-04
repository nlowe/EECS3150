cmake_minimum_required(VERSION 3.2)
project(SynchronousTransmitterReceiver::test)

find_program (BASH_PROGRAM bash)
if (NOT BASH_PROGRAM)
    message(WARNING "Could not find bash. Will not be able to run e2e tests")
endif()

enable_testing()
add_test(
    SynchronousTransmitterReceiver::e2e
    ${BASH_PROGRAM}
    "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
    "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
    "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
)