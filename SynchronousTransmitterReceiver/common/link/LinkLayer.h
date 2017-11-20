#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H

#include <memory>
#include <utility>
#include "../phy/IPhysicalLayer.h"
#include "../phy/NetworkBasedPhysicalLayer.h"
#include <vector>
#include "ErrorCorrectionType.h"

namespace libsts::link
{
    /**
     * An abstraction representing the link layer in the OSI Network Model
     */
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

        /**
         * @return the type of error correction used on the link
         */
        ErrorCorrectionType getErrorCorrectionType() const;

        /**
         * Set the type of error correction to use when communicating on the link
         *
         * @param errorCorrectionType the type of error correction to use
         */
        void setErrorCorrectionType(ErrorCorrectionType errorCorrectionType);

    private:
        std::shared_ptr<libsts::phy::IPhysicalLayer> phy{};
        bool closed = true;
        ErrorCorrectionType errorCorrectionType = ErrorCorrectionType::HAMMING;

        /**
         * Write a frame containing the specified payload with a CRC checksum
         *
         * @param buff the payload to write
         * @param len the length of the payload to write
         */
        void writeCRC(const char* buff, size_t len);
        /**
         * Write a frame containing the specified payload encoded with a hamming code
         *
         * @param buff the payload to write
         * @param len the length of the payload to write
         */
        void writeHamming(const char* buff, size_t len);

        /**
         * Read frames from the frame buffer that are encoded with a hamming code. Reads until no more
         * full frames are left in the frame buffer.
         *
         * @param data the data buffer to place payload bytes into
         * @param frame the frame buffer to read from
         * @return true if the final frame in the buffer contained exactly one payload byte: 0x17
         */
        bool readFrameCRC(std::vector<char>& data, std::vector<char>& frame);
        /**
         * Read frames from the frame buffer, verifying the CRC checksum. Reads until no more
         * full frames are left in the frame buffer.
         *
         * @param data the data buffer to place payload bytes into
         * @param frame the frame buffer to read from
         * @return true if the final frame in the buffer contained exactly one payload byte: 0x17
         */
        bool readFrameHamming(std::vector<char>& data, std::vector<char>& frame);
    };

    /**
     * Create a network-based link layer operating as a server listening on the specified port
     *
     * @param port the port to listen on
     * @return a shared pointer to the constructed link layer
     */
    std::shared_ptr<libsts::link::LinkLayer> CreateServer(const uint16_t port)
    {
        auto phy = std::make_shared<libsts::phy::NetworkBasedPhysicalLayer>(port);
        auto link = std::make_shared<libsts::link::LinkLayer>(phy);

        return link;
    }

    /**
     * Create a network-based link layer operating as a client connecting to the specifed remote
     * on the specified port
     *
     * @param server the remote hostname to connect to
     * @param port the port to connect on
     * @return a shared pointer to the constructed link layer
     */
    std::shared_ptr<libsts::link::LinkLayer> CreateClient(const std::string &server, const uint16_t port)
    {
        auto phy = std::make_shared<libsts::phy::NetworkBasedPhysicalLayer>(server, port);
        auto link = std::make_shared<libsts::link::LinkLayer>(phy);

        return link;
    }
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H
