#include <vector>
#include "LinkLayer.h"
#include "../libsts_except.h"

#define SYN 0x16

namespace libsts::link
{
    void LinkLayer::open()
    {
        if(!closed) return;

        phy->open();
        closed = false;
    }

    void LinkLayer::close()
    {
        if(closed) return;

        phy->flush();
        phy->close();
        closed = true;
    }

    void LinkLayer::write(const char *buff, size_t len)
    {
        if(GetDirection() != libsts::Direction::WRITE) throw libsts::LinkDirectionException("Stream is read only");

        // Chunk the data into 64-byte blocks
        auto remainingBytes = len;
        // Each frame is at most 2 SYN + Size + 64 Data bytes
        auto frame = new char[64 + 3];
        for(auto off = 0; off < len; off += 64)
        {
            frame[0] = SYN;
            auto blockSize = std::min(remainingBytes, 64ul);
            remainingBytes -= blockSize;

            frame[1] = reinterpret_cast<uint8_t&>(blockSize);

            auto i = 0;
            do
            {
                frame[2 + i] = buff[off + i++];
            }while(i < blockSize);

            frame[2 + i++] = SYN;

            try
            {
                phy->write(frame, 2 + i);
            }
            catch(std::exception &ex)
            {
                delete[] frame;
                throw;
            }
        }

        delete[] frame;
    }

    char* LinkLayer::readAll(size_t &len)
    {
        if(GetDirection() != libsts::Direction::READ) throw libsts::LinkDirectionException("Stream is write only");

        std::vector<char> dataBuffer;
        auto frameBuffer = new char[64 + 3];

        while(phy->read(frameBuffer, 64 + 3) == 64 + 3)
        {
            for(auto i = 2; i < 64 + 3; i++)
            {
                dataBuffer.insert(dataBuffer.end(), frameBuffer[i]);
            }
        }

        auto buff = new char[dataBuffer.size()];
        std::copy(dataBuffer.begin(), dataBuffer.end(), buff);
        return buff;
    }
}