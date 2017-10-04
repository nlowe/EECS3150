#include <vector>
#include <iostream>
#include "LinkLayer.h"
#include "LinkLayerExceptions.h"

#define SYN 0x16
#define FRAME_MAX_PAYLOAD_LEN 64ul
#define FRAME_MAX_LEN (FRAME_MAX_PAYLOAD_LEN + 3)

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
        // Chunk the data into 64-byte blocks
        auto remainingBytes = len;
        // Each frame is at most 2 SYN + Size + 64 Data bytes
        auto frame = new char[FRAME_MAX_LEN]{0};
        for(auto off = 0; off < len; off += FRAME_MAX_PAYLOAD_LEN)
        {
            frame[0] = SYN;
            auto blockSize = std::min(remainingBytes, FRAME_MAX_PAYLOAD_LEN);
            remainingBytes -= blockSize;

            frame[1] = reinterpret_cast<uint8_t&>(blockSize);

            auto i = 0;
            do
            {
                frame[2 + i++] = buff[off + i];
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
        std::vector<char> dataBuffer;
        std::vector<char> frameBuffer;
        frameBuffer.reserve(2 * FRAME_MAX_LEN);

        size_t chunkSize = 0;
        do
        {
            // Allocate a buffer for copying bytes into the frame buffer
            char buff[FRAME_MAX_LEN]{0};
            chunkSize = phy->read(buff, FRAME_MAX_LEN);
            frameBuffer.insert(frameBuffer.end(), std::begin(buff), std::end(buff));

            if(frameBuffer[0] != SYN) throw FramingException("Expected start-of-frame at byte " + std::to_string(dataBuffer.size()) + " but got " + std::to_string(frameBuffer[0]));

            // Extract the payload from the frame into the data buffer
            dataBuffer.insert(dataBuffer.end(), frameBuffer.begin() + 2, frameBuffer.begin() + 2 + frameBuffer[1]);

            if(frameBuffer[2 + frameBuffer[1]] != SYN) throw FramingException("Expected end-of-frame at byte " + std::to_string(dataBuffer.size() + chunkSize) + " but got " + std::to_string(frameBuffer[2 + frameBuffer[1]]));

            // Erase the frame we just decoded. If we didn't have a FRAME_MAX_LEN sized frame we have the starting part
            // of the next frame in the buffer left-over
            frameBuffer.erase(frameBuffer.begin(), frameBuffer.begin() + frameBuffer[1] + 3);
        }while(!phy->eof());

        // Specify the length and copy the data to a buffer to return
        len = dataBuffer.size();
        auto buff = new char[dataBuffer.size()];

        std::copy(dataBuffer.begin(), dataBuffer.end(), buff);
        return buff;
    }
}