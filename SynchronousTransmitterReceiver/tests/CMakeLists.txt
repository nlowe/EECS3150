cmake_minimum_required(VERSION 3.2)
project(SynchronousTransmitterReceiver::test)

find_program (BASH_PROGRAM bash)
if (NOT BASH_PROGRAM)
    message(WARNING "Could not find bash. Will not be able to run e2e tests")
endif()

enable_testing()
add_test(
    SynchronousTransmitterReceiver::e2e::7
    ${BASH_PROGRAM}
    "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
    "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
    "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
    "7"
)

add_test(
        SynchronousTransmitterReceiver::e2e::8
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "8"
)

add_test(
        SynchronousTransmitterReceiver::e2e::9
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "9"
)

add_test(
        SynchronousTransmitterReceiver::e2e::63
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "63"
)

add_test(
        SynchronousTransmitterReceiver::e2e::64
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "64"
)

add_test(
        SynchronousTransmitterReceiver::e2e::65
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "65"
)

add_test(
        SynchronousTransmitterReceiver::e2e::127
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "127"
)

add_test(
        SynchronousTransmitterReceiver::e2e::128
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "128"
)

add_test(
        SynchronousTransmitterReceiver::e2e::129
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "129"
)

add_test(
        SynchronousTransmitterReceiver::e2e::222
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/server/server"
        "222"
)

add_test(
        SynchronousTransmitterReceiver::parity
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/parity.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/client/client"
)
