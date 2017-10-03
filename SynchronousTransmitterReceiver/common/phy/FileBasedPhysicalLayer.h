#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_FILEBASEDPHYSICALLAYER_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_FILEBASEDPHYSICALLAYER_H

#include <fstream>
#include <string>
#include <utility>

#include "IPhysicalLayer.h"

namespace libsts::phy
{
    class FileBasedPhysicalLayer : public IPhysicalLayer {
    public:
        explicit FileBasedPhysicalLayer(std::string file, libsts::Direction direction)
                : Direction(direction), file(std::move(file)) {}
        ~FileBasedPhysicalLayer() override;

        void write(const char *data, size_t len) override;
        size_t read(char *buff, size_t len) override;

        void open() override;
        void close() override;

        uint32_t flush() override;

        const std::string GetFile() { return file; }

        const libsts::Direction GetDirection() override
        {
            return Direction;
        }

    private:
        inline const char calculateParity();
        inline const char encodebit(const char& c, uint8_t bit);

        const libsts::Direction Direction;

        const std::string file;
        std::fstream link;
        bool closed = true;

        uint8_t BitInByteCounter = 0;
        uint8_t BitCounter = 0;

        uint8_t ParityBuffer = 0;
        uint8_t BitBuffer = 0;
    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_FILEBASEDPHYSICALLAYER_H
