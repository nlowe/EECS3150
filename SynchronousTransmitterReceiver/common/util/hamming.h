#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_HAMMING_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_HAMMING_H

#define HAMMING_DATA_MASK 0x0717

#include <cstdint>

/**
 * Select the specified bit
 *
 * @tparam T the integer type
 * @param opt the argument to extract the specified bit from
 * @param bit the bit position to extract
 * @return the specified bit shifted down to the least significant bit
 */
template <class T>
inline int bitsel(T opt, uint8_t bit)
{
    return (opt & (1 << bit)) >> bit;
}

/**
 * Encode the provided byte with Hamming 7-4 on each nibble. The high byte in the result is the
 * encoded value of the high nibble of the argument.
 *
 * @param byte the byte to encode
 * @return a 16-bit integer containing each nibble in the argument encoded with hamming 7-4
 */
uint16_t hammingEncode(const char byte)
{
    // MSB of each byte is reserved for parity
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

    // Move data bits around
    auto result = static_cast<uint16_t>(
        (
            (byte & 0x80) << 6 |
            (byte & 0x70) << 4 |
            (byte & 0x08) << 1 |
            (byte & 0x7)
        )
        & HAMMING_DATA_MASK
    );

    // low byte hamming
    result |= (bitsel(result, 2) ^ bitsel(result, 1) ^ bitsel(result, 0)) << 3; // p4
    result |= (bitsel(result, 4) ^ bitsel(result, 1) ^ bitsel(result, 0)) << 5; // p2
    result |= (bitsel(result, 4) ^ bitsel(result, 2) ^ bitsel(result, 0)) << 6; // p1

    // high byte hamming
    result |= (bitsel(result, 10) ^ bitsel(result,  9) ^ bitsel(result, 8)) << 11; // p4
    result |= (bitsel(result, 12) ^ bitsel(result,  9) ^ bitsel(result, 8)) << 13; // p2
    result |= (bitsel(result, 12) ^ bitsel(result, 10) ^ bitsel(result, 8)) << 14; // p1

    return result;
}

/**
 * Bitmask the data bits from the provided hamming-7-4 encoded byte, correcting any
 * single-bit errors. Does not shift data bits.
 *
 * @param byte the byte to correct
 * @return the data bits from the hamming-7-4 encoded byte
 */
uint8_t hammingCorrect(const char byte)
{
    // 07 06 05 04 03 02 01 00
    // 00 00 00 00 d3 d2 d1 d0
    //
    //       expands to
    //
    // __ p1 p2 d3 p4 d2 d1 d0
    //    xx    xx    xx    xx
    //       xx xx       xx xx
    //             xx xx xx xx

    auto c = (bitsel(byte, 6) ^ bitsel(byte, 4) ^ bitsel(byte, 2) ^ bitsel(byte, 0)) |
             (bitsel(byte, 5) ^ bitsel(byte, 4) ^ bitsel(byte, 1) ^ bitsel(byte, 0)) << 1 |
             (bitsel(byte, 3) ^ bitsel(byte, 2) ^ bitsel(byte, 1) ^ bitsel(byte, 0)) << 2;

    auto result = byte;

    if (c != 0)
    {
        result ^= 1 << (8 - c - 1);
    }

    return static_cast<uint8_t>(result & 0b00010111);
}

/**
 * Decode the provided high and low byte which are encoded with hamming-7-4
 *
 * @param highByte the high byte in the encoded message
 * @param lowByte the low byte in the encoded message
 * @return the corrected data bits from the encoded bytes
 */
uint8_t hammingDecode(const char highByte, const char lowByte)
{
    auto correctedHighByte = hammingCorrect(highByte);
    auto correctedLowByte  = hammingCorrect(lowByte);

    return static_cast<uint8_t>(
        (correctedHighByte & 0x10) << 3 | (correctedHighByte & 0x07) << 4 |
        (correctedLowByte  & 0x10) >> 1 | (correctedLowByte  & 0x07)
    );
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_HAMMING_H
