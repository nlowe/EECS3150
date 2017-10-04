#include "FileBasedPhysicalLayer.h"
#include "PhysicalLayerException.h"

namespace libsts::phy
{
    FileBasedPhysicalLayer::~FileBasedPhysicalLayer()
    {
        if(link.is_open()) link.close();
    }

    uint32_t FileBasedPhysicalLayer::flush()
    {
        if (closed) throw libsts::phy::NotConnectedException("The communication file is not open");

        link.flush();
        link.seekg(0);
        link.seekp(0);

        return 0;
    }

    void FileBasedPhysicalLayer::open()
    {
        if(!closed) return;

        link.open(file, std::ios::in | std::ios::out | std::ios::app);
        if(!link.good())
        {
            perror("Could not open communication file: ");
            throw libsts::phy::PhysicalLayerException("Could not open communication file " + file);
        }

        closed = false;
    }

    void FileBasedPhysicalLayer::close()
    {
        if (closed) return;

        link.close();

        closed = true;
    }

    inline const char calculateParity(uint8_t byte)
    {
        byte ^= byte >> 4;
        byte ^= byte >> 2;
        byte ^= byte >> 1;

        return ((~byte) & 1) == 1 ? '1' : '0';
    }

    inline const char encodebit(const char& c, uint8_t bit)
    {
        return (c & (1 << bit)) >> bit == 1 ? '1' : '0';
    }

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

    void FileBasedPhysicalLayer::write(const char *data, size_t len)
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

        // Write the expanded bits to the stream
        link.write(stupidEncoding, len * 8);
        delete[] stupidEncoding;
    }

    size_t FileBasedPhysicalLayer::read(char *buff, size_t len)
    {
        // Each byte becomes 8 "bits": 7 data bits (with the MSB of the byte = 0) and a parity bit
        auto stupidEncoding = new char[len * 8];

        link.read(stupidEncoding, len * 8);
        auto bitlen = link.gcount();
        auto realLen = static_cast<size_t>(bitlen / 8);

        for(auto i = 0; i < realLen; i++)
        {
            uint8_t byte = 0;

            byte |= decodebit(stupidEncoding[(i * 8) + 0], 0);
            byte |= decodebit(stupidEncoding[(i * 8) + 1], 1);
            byte |= decodebit(stupidEncoding[(i * 8) + 2], 2);
            byte |= decodebit(stupidEncoding[(i * 8) + 3], 3);
            byte |= decodebit(stupidEncoding[(i * 8) + 4], 4);
            byte |= decodebit(stupidEncoding[(i * 8) + 5], 5);
            byte |= decodebit(stupidEncoding[(i * 8) + 6], 6);

            if(calculateParity(byte) != stupidEncoding[(i * 8) + 7])
            {
                throw ParityCheckFailure("Parity check failed for byte " + std::to_string(i));
            }

            buff[i] = byte;
        }

        delete[] stupidEncoding;
        return realLen;
    }

    bool FileBasedPhysicalLayer::eof() {
        return link.eof();
    }
}

