#include <iostream>
#include "FileBasedPhysicalLayer.h"
#include "PhysicalLayerException.h"
#include "StupidEncoding.h"

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

        if(direction == libsts::Direction::WRITE)
        {
            link.open(file, std::ios::out | std::ios::trunc);
        }
        else
        {
            link.open(file, std::ios::in);
        }

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


    void FileBasedPhysicalLayer::write(const char *data, size_t len)
    {
        if (direction != libsts::Direction::WRITE) throw libsts::BadDirectionException("Channel is not open for write");

        size_t res;
        auto stupidEncoding = libsts::phy::StupidEncoding::expandBitsToBytes(data, len, res);

        // Write the expanded bits to the stream
        link.write(stupidEncoding, res);
        delete[] stupidEncoding;
    }

    size_t FileBasedPhysicalLayer::read(char *buff, size_t len)
    {
        if (direction != libsts::Direction::READ) throw libsts::BadDirectionException("Channel is not open for read");

        auto stupidEncoding = new char[len * 8];
        link.read(stupidEncoding, len * 8);

        size_t realLen;
        auto decoded = libsts::phy::StupidEncoding::collapseBytesToBits(stupidEncoding, len * 8, realLen);

        std::copy(decoded, decoded + realLen, buff);

        delete[] decoded;
        delete[] stupidEncoding;
        return realLen;
    }

    bool FileBasedPhysicalLayer::eof() {
        return link.eof();
    }

    libsts::Direction FileBasedPhysicalLayer::getDirection() {
        return direction;
    }
}
