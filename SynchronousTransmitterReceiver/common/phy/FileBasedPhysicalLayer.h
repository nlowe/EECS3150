#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_FILEBASEDPHYSICALLAYER_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_FILEBASEDPHYSICALLAYER_H

#include <fstream>
#include <string>
#include <utility>

#include "IPhysicalLayer.h"

namespace libsts::phy
{
    /**
     * A physical layer whose transmission medium is a file. Each byte
     * passed to this layer is expanded into '0' and '1' for each bit.
     * The MSB of each byte is overwritten with a parity bit (using
     * odd parity). Thus, if any byte passed to this layer has the MSB
     * set, an IllegalEncodingException will be thrown.
     */
    class FileBasedPhysicalLayer : public IPhysicalLayer {
    public:
        FileBasedPhysicalLayer(const std::string &file, const libsts::Direction direction)
                : direction(direction),  file(std::move(file)) {}
        ~FileBasedPhysicalLayer() override;

        void write(const char *data, size_t len) override;
        size_t read(char *buff, size_t len) override;

        void open() override;
        void close() override;

        uint32_t flush() override;

        const std::string GetFile() { return file; }

        bool eof() override;

        Direction getDirection() override;

    private:
        const libsts::Direction direction{};
        const std::string file;
        std::fstream link;
        bool closed = true;
    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_FILEBASEDPHYSICALLAYER_H
