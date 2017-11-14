#include <vector>
#include <iostream>
#include "LinkLayer.h"
#include "LinkLayerExceptions.h"
#include "../util/crc-16-ansi.h"
#include "../util/hamming.h"

#define SYN 0x16
#define ETB 0x17
#define FRAME_MAX_PAYLOAD_LEN 64ul
#define FRAME_MAX_LEN_CRC (FRAME_MAX_PAYLOAD_LEN + 3)
#define FRAME_MAX_LEN_HAMMING (3 + FRAME_MAX_PAYLOAD_LEN * 2)

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

    void LinkLayer::writeCRC(const char *buff, size_t len)
    {
        // Each frame is at most 2 SYN + Size + 64 Data bytes + 2 bytes for CRC
        auto frame = new char[FRAME_MAX_LEN_CRC + 2]{0};
        frame[0] = SYN;
        frame[1] = reinterpret_cast<uint8_t&>(len);

        auto i = 0;
        do
        {
            frame[2 + i++] = buff[i];
        }while(i < len);

        auto crc = crc16ansi(buff, len);
        frame[2 + i++] = static_cast<uint8_t>(crc >> 8);
        frame[2 + i++] = static_cast<uint8_t>(crc & 0xff);
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

    void LinkLayer::writeHamming(const char *buff, size_t len)
    {
        // Each frame is at most 2 SYN + Size + 64 Data bytes
        // Hamming requires two bytes per payload byte
        auto frame = new char[FRAME_MAX_LEN_HAMMING]{0};
        frame[0] = SYN;
        frame[1] = reinterpret_cast<uint8_t&>(len);

        auto i = 0;
        do
        {
            auto hamming = hammingEncode(buff[i]);
            frame[2 + i++] = static_cast<uint8_t>(hamming >> 8);
            frame[2 + i++] = static_cast<uint8_t>(hamming & 0xff);
        }while(i/2 < len);

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

    void LinkLayer::write(const char *buff, size_t len)
    {
        // Chunk the data into 64-byte blocks
        auto remainingBytes = len;
        for(auto off = 0; off < len; off += FRAME_MAX_PAYLOAD_LEN)
        {
            auto blockSize = std::min(remainingBytes, FRAME_MAX_PAYLOAD_LEN);
            remainingBytes -= blockSize;

            switch(getErrorCorrectionType())
            {
                case ErrorCorrectionType::CRC_ANSI_16: writeCRC(buff + off, blockSize); break;
                case ErrorCorrectionType::HAMMING: writeHamming(buff + off, blockSize); break;
            }
        }

        // Also send ETB
        char FinalFrame[] = {SYN, 1, ETB, SYN};
        switch(getErrorCorrectionType())
        {
            case ErrorCorrectionType::CRC_ANSI_16: writeCRC(FinalFrame, 4); break;
            case ErrorCorrectionType::HAMMING: writeHamming(FinalFrame, 4); break;
        }
    }

    bool LinkLayer::readFrameCRC(size_t chunkSize, std::vector<char>& data, std::vector<char>& frame)
    {
        // TODO: Correct offset
        if(frame[4 + frame[1]] != SYN) throw FramingException("Expected end-of-frame at byte " + std::to_string(data.size() + chunkSize) + " but got " + std::to_string(frame[4 + frame[1]]));

        

        // Erase the frame we just decoded. If we didn't have a FRAME_MAX_LEN_CRC sized frame we have the starting part
        // of the next frame in the buffer left-over
        frame.erase(frame.begin(), frame.begin() + frame[1] + 5);
        return false;

    }

    bool LinkLayer::readFrameHamming(size_t chunkSize, std::vector<char>& data, std::vector<char>& frame)
    {
        // TODO: Correct offset
        if(frame[2 + frame[1] * 2] != SYN) throw FramingException("Expected end-of-frame at byte " + std::to_string(data.size() + chunkSize) + " but got " + std::to_string(frame[2 + 2 * frame[1]]));


        // Erase the frame we just decoded. If we didn't have a FRAME_MAX_LEN_HAMMING sized frame we have the starting part
        // of the next frame in the buffer left-over
        frame.erase(frame.begin(), frame.begin() + frame[1] * 2 + 3);
        return false;
    }

    char* LinkLayer::readAll(size_t &len)
    {
        std::vector<char> dataBuffer;
        std::vector<char> frameBuffer;
        frameBuffer.reserve(2 * FRAME_MAX_LEN_CRC);

        bool isFinalFrameInBlock = false;
        size_t chunkSize = 0;
        do
        {
            // Allocate a buffer for copying bytes into the frame buffer
            // CRC frames will fit into hamming frames
            char buff[FRAME_MAX_LEN_HAMMING]{0};
            chunkSize = phy->read(buff, FRAME_MAX_LEN_HAMMING);
            if(chunkSize == 0) break;

            frameBuffer.insert(frameBuffer.end(), std::begin(buff), std::end(buff));

            if(frameBuffer[0] != SYN) throw FramingException("Expected start-of-frame at byte " + std::to_string(dataBuffer.size()) + " but got " + std::to_string(frameBuffer[0]));

            // Extract the payload from the frame into the data buffer
            switch(getErrorCorrectionType())
            {
                case libsts::link::ErrorCorrectionType::CRC_ANSI_16: isFinalFrameInBlock = readFrameCRC(chunkSize, dataBuffer, frameBuffer); break;
                case libsts::link::ErrorCorrectionType::HAMMING: isFinalFrameInBlock = readFrameHamming(chunkSize, dataBuffer, frameBuffer); break;
            }
        }while(!phy->eof() && chunkSize > 0 && !isFinalFrameInBlock);

        // Specify the length and copy the data to a buffer to return
        len = dataBuffer.size();
        auto buff = new char[dataBuffer.size()];

        std::copy(dataBuffer.begin(), dataBuffer.end(), buff);
        return buff;
    }

    ErrorCorrectionType LinkLayer::getErrorCorrectionType() const {
        return errorCorrectionType;
    }

    void LinkLayer::setErrorCorrectionType(ErrorCorrectionType errorCorrectionType) {
        LinkLayer::errorCorrectionType = errorCorrectionType;
    }
}