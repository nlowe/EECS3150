//
// Created by nathan on 9/21/17.
//

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
        explicit FileBasedPhysicalLayer(std::string file) : file(std::move(file)) {}
        ~FileBasedPhysicalLayer() override;

        void write(const char *buff, size_t len) override;

        size_t read(char *&buff, size_t count) override;

        void open() override;
        void close() override;

        uint32_t flush() override;

        const std::string GetFile() { return file; }

    private:
        const std::string file;
        std::fstream link;
        bool closed = true;
    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_FILEBASEDPHYSICALLAYER_H
