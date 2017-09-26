//
// Created by nathan on 9/21/17.
//

#include "FileBasedPhysicalLayer.h"
#include "PhysicalLayerException.h"

namespace libsts::phy
{
    FileBasedPhysicalLayer::~FileBasedPhysicalLayer() {
        if(link.is_open()) link.close();
    }

    uint32_t FileBasedPhysicalLayer::flush() {
        if (closed) throw libsts::phy::NotConnectedException("The communication file is not open");

        link.flush();
        link.seekg(0);
        link.seekp(0);

        return 0;
    }

    void FileBasedPhysicalLayer::open() {
        if(!closed) return;

        link.open(file, std::ios::in | std::ios::out);
        if(!link.good()) throw libsts::phy::PhysicalLayerException("Could not open communication file " + file);

        closed = false;
    }

    void FileBasedPhysicalLayer::close() {
        if (closed) return;

        link.close();

        closed = true;
    }


    inline const char encodebit(const char c, uint8_t bit)
    {
        return (c & (1 << bit)) >> bit == 1 ? '1' : '0';
    }

    inline void decodebit(const char bit, uint8_t off, char& result)
    {
        result |= (bit == '1') << off;
    }

    void FileBasedPhysicalLayer::write(const char *buff, size_t len) {
        auto stupidEncoding = new char[len*8]{0};

        // Expand each bit into a '0' or '1'
        for(auto i = 0; i < len; i++)
        {
            auto byte = buff[i];

            stupidEncoding[(i*8) + 0] = encodebit(byte, 0);
            stupidEncoding[(i*8) + 1] = encodebit(byte, 1);
            stupidEncoding[(i*8) + 2] = encodebit(byte, 2);
            stupidEncoding[(i*8) + 3] = encodebit(byte, 3);
            stupidEncoding[(i*8) + 4] = encodebit(byte, 4);
            stupidEncoding[(i*8) + 5] = encodebit(byte, 5);
            stupidEncoding[(i*8) + 6] = encodebit(byte, 6);
            stupidEncoding[(i*8) + 7] = encodebit(byte, 7);
        }

        link.write(stupidEncoding, len * 8);

        delete[] stupidEncoding;
    }

    size_t FileBasedPhysicalLayer::read(char *&buff, size_t count) {

        auto stupidEncoding = new char[count*8]{0};

        auto beg = link.tellg();
        link.read(stupidEncoding, count * 8);
        auto read = static_cast<size_t>(link.tellg() - beg) / 8;

        // Collapse '0' and '1' characters into bytes
        for(auto i = 0; i < read; i++)
        {
            char byte = 0;
            decodebit(stupidEncoding[(i * 8) + 0], 0, byte);
            decodebit(stupidEncoding[(i * 8) + 1], 1, byte);
            decodebit(stupidEncoding[(i * 8) + 2], 2, byte);
            decodebit(stupidEncoding[(i * 8) + 3], 3, byte);
            decodebit(stupidEncoding[(i * 8) + 4], 4, byte);
            decodebit(stupidEncoding[(i * 8) + 5], 5, byte);
            decodebit(stupidEncoding[(i * 8) + 6], 6, byte);
            decodebit(stupidEncoding[(i * 8) + 7], 7, byte);

            buff[i] = byte;
        }

        delete[] stupidEncoding;

        return read;
    }
}

