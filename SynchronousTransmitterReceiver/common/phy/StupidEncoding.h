#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_STUPIDENCODING_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_STUPIDENCODING_H

#include <cstddef>
#include <cstdint>
#include "PhysicalLayerException.h"

namespace libsts::phy::StupidEncoding
{
    /**
     * Calculates the odd parity for the specified byte
     *
     * @param byte the byte to check
     * @return '1' if the parity bit should be set, '0' otherwise
     */
    inline const char calculateParity(uint8_t byte)
    {
        byte ^= byte >> 4;
        byte ^= byte >> 2;
        byte ^= byte >> 1;

        return ((~byte) & 1) == 1 ? '1' : '0';
    }

    /**
     * Encodes the specified bit in the provided byte to a '0' or '1'
     *
     * @param c the byte to encode from
     * @param bit the bit to encode
     * @return '1' if the specified bit in the byte was set, '0' otherwise
     */
    inline const char encodebit(const char& c, uint8_t bit)
    {
        return (c & (1 << bit)) >> bit == 1 ? '1' : '0';
    }

    /**
     * Decode the specified bit
     *
     * @param c the "bit" to decode
     * @param bit the position of the bit in the byte
     * @return (1 << bit) if c == '1', 0 otherwise
     */
    inline const uint8_t decodebit(const char& c, uint8_t bit)
    {
        if (c == '1')
        {
            return static_cast<const uint8_t>(1 << bit);
        }
        else if(c == '0')
        {
            return 0;
        }
        else
        {
            throw IllegalEncodingException("Expected a '1' or '0' but got a " + std::to_string(c));
        }
    }

    char* expandBitsToBytes(const char* data, size_t len, size_t &res)
    {
        // Each byte becomes 8 "bits": 7 data bits (with the MSB of the byte = 0) and a parity bit
        auto stupidEncoding = new char[len * 8];

        for(auto i = 0; i < len; i++)
        {
            auto byte = reinterpret_cast<uint8_t&>(const_cast<char&>(data[i]));

            if(byte & 0x80) throw IllegalEncodingException("This project only supports 7-bit ascii");
            byte &= 0x7f;

            stupidEncoding[(i * 8) + 0] = encodebit(byte, 0);
            stupidEncoding[(i * 8) + 1] = encodebit(byte, 1);
            stupidEncoding[(i * 8) + 2] = encodebit(byte, 2);
            stupidEncoding[(i * 8) + 3] = encodebit(byte, 3);
            stupidEncoding[(i * 8) + 4] = encodebit(byte, 4);
            stupidEncoding[(i * 8) + 5] = encodebit(byte, 5);
            stupidEncoding[(i * 8) + 6] = encodebit(byte, 6);
            stupidEncoding[(i * 8) + 7] = calculateParity(byte);
        }

        return stupidEncoding;
    }

    char* collapseBytesToBits(const char* data, size_t len, size_t &res)
    {
        // Each byte becomes 8 "bits": 7 data bits (with the MSB of the byte = 0) and a parity bit
        res = len / 8;
        auto stupidEncoding = new char[res]{0};

        for(auto i = 0; i < res; i++)
        {
            uint8_t byte = 0;

            byte |= decodebit(data[(i * 8) + 0], 0);
            byte |= decodebit(data[(i * 8) + 1], 1);
            byte |= decodebit(data[(i * 8) + 2], 2);
            byte |= decodebit(data[(i * 8) + 3], 3);
            byte |= decodebit(data[(i * 8) + 4], 4);
            byte |= decodebit(data[(i * 8) + 5], 5);
            byte |= decodebit(data[(i * 8) + 6], 6);

            if(calculateParity(byte) != data[(i * 8) + 7])
            {
                throw ParityCheckFailure("Parity check failed for byte " + std::to_string(i));
            }

            stupidEncoding[i] = byte;
        }

        return stupidEncoding;
    }
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_STUPIDENCODING_H
