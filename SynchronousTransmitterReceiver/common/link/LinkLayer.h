#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H


#include <memory>
#include <utility>
#include "../phy/IPhysicalLayer.h"
#include "../phy/FileBasedPhysicalLayer.h"

/**
 * An abstraction representing the link layer in the OSI Network Model
 */
namespace libsts::link
{
    class LinkLayer {
    public:
        explicit LinkLayer(std::shared_ptr<libsts::phy::IPhysicalLayer> phy)
            : phy(std::move(phy))
        {
        }

        /**
         * Open the link
         */
        void open();
        /**
         * Close the link
         */
        void close();

        /**
         * Write the specified buffer out to the channel
         *
         * @param buff the data to write
         * @param len the number of bytes in the buffer
         */
        void write(const char* buff, size_t len);
        /**
         * Read as many bytes as possible from the link
         *
         * @param len Populated with the total number of data bytes read
         * @return a shared pointer to the data
         */
        char* readAll(size_t &len);

        const libsts::Direction getDirection()
        {
            return phy->getDirection();
        }

    private:
        std::shared_ptr<libsts::phy::IPhysicalLayer> phy{};
        bool closed = true;
    };

    std::shared_ptr<libsts::link::LinkLayer> CreateFileBasedReader(const std::string &file)
    {
        auto phy = std::make_shared<libsts::phy::FileBasedPhysicalLayer>(file, libsts::Direction::READ);
        auto link = std::make_shared<libsts::link::LinkLayer>(phy);

        return link;
    }

    std::shared_ptr<libsts::link::LinkLayer> CreateFileBasedWriter(const std::string &file)
    {
        auto phy = std::make_shared<libsts::phy::FileBasedPhysicalLayer>(file, libsts::Direction::WRITE);
        auto link = std::make_shared<libsts::link::LinkLayer>(phy);

        return link;
    }
}


#endif //SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H
