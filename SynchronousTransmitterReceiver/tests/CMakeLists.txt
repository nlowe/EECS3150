cmake_minimum_required(VERSION 3.2)
project(SynchronousTransmitterReceiver::test)

find_program (BASH_PROGRAM bash)
if (NOT BASH_PROGRAM)
    message(WARNING "Could not find bash. Will not be able to run e2e tests")
endif()

enable_testing()
add_test(
        SynchronousTransmitterReceiver::e2e::crc::7
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "7"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::8
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "8"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::9
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "9"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::63
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "63"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::64
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "64"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::65
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "65"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::127
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "127"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::128
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "128"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::129
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "129"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::crc::222
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "222"
        "-ec"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::7
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "7"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::8
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "8"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::9
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "9"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::63
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "63"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::64
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "64"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::65
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "65"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::127
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "127"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::128
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "128"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::129
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "129"
        "-eh"
)

add_test(
        SynchronousTransmitterReceiver::e2e::hamming::222
        ${BASH_PROGRAM}
        "${CMAKE_CURRENT_SOURCE_DIR}/e2e.sh"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkClient/networkClient"
        "${CMAKE_BINARY_DIR}/SynchronousTransmitterReceiver/networkServer/networkServer"
        "222"
        "-eh"
)