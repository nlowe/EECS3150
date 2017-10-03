#include <unitypes.h>
#include "FileBasedPhysicalLayer.h"
#include "PhysicalLayerException.h"
#include "../libsts_except.h"

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

        link.open(file, std::ios::in | std::ios::out | std::ios::trunc);
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

    inline const char FileBasedPhysicalLayer::calculateParity()
    {
        ParityBuffer ^= ParityBuffer >> 4;
        ParityBuffer ^= ParityBuffer >> 2;
        ParityBuffer ^= ParityBuffer >> 1;

        return ((~ParityBuffer) & 1) == 1 ? '1' : '0';
    }

    inline const char FileBasedPhysicalLayer::encodebit(const char& c, uint8_t bit)
    {
        auto v = (c & (1 << bit)) >> bit;
        ParityBuffer |= v;
        ParityBuffer <<= 1;
        return v == 1 ? '1' : '0';
    }

    void FileBasedPhysicalLayer::write(const char *data, size_t len)
    {
        if(Direction != libsts::Direction::WRITE) throw libsts::LinkDirectionException("Stream is read only");

        // Each byte becomes 8 "bits" and for every 7 "bits" we have a checksum bit
        auto bits = len * 8;
        auto bytes = bits + bits / 7;
        auto stupidEncoding = new char[bytes];

        auto r = 0;
        auto w = 0;
        auto d = 0;

        while(r < len)
        {
            auto byte = data[r++];

            for(uint8_t i = 0; i < 8; i++)
            {
                stupidEncoding[w++] = encodebit(byte, i);
                d++;
                if(d % 7 == 0)
                {
                    stupidEncoding[w++] = calculateParity();
                    ParityBuffer = 0;
                }
            }
        }

        // Write the expanded bits to the stream
        link.write(stupidEncoding, bytes);
        delete[] stupidEncoding;
    }

    size_t FileBasedPhysicalLayer::read(char *buff, size_t len)
    {
        if(Direction != libsts::Direction::READ) throw libsts::LinkDirectionException("Stream is write only");

        // Each byte becomes 8 "bits" and for every 7 "bits" we have a checksum bit
        auto bits = len * 8;
        auto bytes = bits + bits / 7;
        auto stupidEncoding = new char[bytes];

        auto pos = link.tellg();
        link.read(stupidEncoding, bytes);
        auto size = link.tellg() - pos;

        auto r = 0;
        size_t w = 0;

        while(r < size)
        {
            auto bit = stupidEncoding[r++];
            if(bit == '1')
            {
                BitBuffer |= 1;
                ParityBuffer |= 1;
                ParityBuffer <<= 1;
            }
            else if(bit != '0')
            {
                throw PhysicalLayerException("Illegal Encoding. Expected a '0' or '1' but got " + bit);
            }

            if(++BitInByteCounter == 8)
            {
                buff[w++] = BitBuffer;
                BitInByteCounter = BitBuffer = 0;
            }
            else
            {
                BitBuffer <<= 1;
                if(BitCounter % 7 == 0)
                {
                    if(r < size)
                    {
                        auto parityBit = stupidEncoding[r++];

                        if(calculateParity() != parityBit)
                        {
                            delete[] stupidEncoding;
                            throw PhysicalLayerException("Parity Check Failed");
                        }

                        ParityBuffer = 0;
                    }
                    else
                    {
                        delete[] stupidEncoding;
                        throw PhysicalLayerException("Not enough bits for parity check");
                    }
                }
            }

            if(++BitCounter == 64) BitCounter = 0;
        }

        delete[] stupidEncoding;
        return w;
    }
}

