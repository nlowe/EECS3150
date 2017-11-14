//
// Created by nathan on 11/12/17.
//

#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_HAMMING_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_HAMMING_H

#define HAMMING_DATA_MASK 0x0717

#include <cstdint>

uint16_t hammingEncode(const char byte)
{
    // MSB of each byte is reserved for parity
    uint16_t result = reinterpret_cast<uint8_t&>(const_cast<char&>(byte));

    // for simplicity, we just do hamming 7-4 twice which still allows
    // us to do an overall parity bit

    // 15 14 13 12 11 10 09 08    07 06 05 04 03 02 01 00
    //                            00 d6 d5 d4 d3 d2 d1 d0
    //
    //                     expands to
    //
    // __ p1 p2 00 p4 d6 d5 d4    __ p1 p2 d3 p4 d2 d1 d0
    //    xx    xx    xx    xx       xx    xx    xx    xx
    //       xx xx       xx xx          xx xx       xx xx
    //             xx xx xx xx                xx xx xx xx
    // /\       /\                /\
    // parity  padding            parity

    result = static_cast<uint16_t>(((result & 0x70) << 4) & HAMMING_DATA_MASK);

    // low byte hamming
    result |= ((result & 1 << 2) ^ (result & 1 << 1) ^ (result & 1)) << 3;
    result |= ((result & 1 << 4) ^ (result & 1 << 1) ^ (result & 1)) << 5;
    result |= ((result & 1 << 4) ^ (result & 1 << 2) ^ (result & 1)) << 6;

    // high byte hamming
    result |= ((result & 1 << 10) ^ (result & 1 <<  9) ^ (result & 1 << 8)) << 11;
    result |= ((result & 1 << 12) ^ (result & 1 <<  9) ^ (result & 1 << 8)) << 13;
    result |= ((result & 1 << 12) ^ (result & 1 << 10) ^ (result & 1 << 8)) << 14;

    return result;
}

uint8_t hammingDecode(uint16_t byte)
{
    // TODO: implement
    return 0;
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_HAMMING_H
