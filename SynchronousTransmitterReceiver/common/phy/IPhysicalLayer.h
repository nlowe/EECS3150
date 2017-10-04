#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_IPHYSICALLAYER_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_IPHYSICALLAYER_H


#include <cstdint>
#include <memory>

namespace libsts::phy
{
    /**
     * An abstraction of the physical layer in the OSI Networking Model
     */
    class IPhysicalLayer {
    public:
        IPhysicalLayer() = default;
        virtual ~IPhysicalLayer() = default;

        /**
         * Open a physical communication channel
         */
        virtual void open() = 0;
        /**
         * Close the physical communication channel
         */
        virtual void close() = 0;

        /**
         * Write data to the physical link
         *
         * @param data The data buffer to write
         * @param len how many bytes are in the buffer
         */
        virtual void write(const char* data, size_t len) = 0;
        /**
         * Attempt to read the requested number of bytes from the link
         *
         * @param buff the buffer to read into
         * @param len the number of bytes to read
         * @return the actual number of bytes read
         */
        virtual size_t read(char* buff, size_t len) = 0;

        /**
         * Write the buffer to the physical device
         *
         * @return the number of bytes written
         */
        virtual uint32_t flush() = 0;

        /**
         * @return true if there is no more data on the link
         */
        virtual bool eof() = 0;
    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_IPHYSICALLAYER_H
