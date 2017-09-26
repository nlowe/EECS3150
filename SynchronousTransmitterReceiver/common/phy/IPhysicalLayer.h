//
// Created by nathan on 9/21/17.
//

#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_IPHYSICALLAYER_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_IPHYSICALLAYER_H


#include <cstdint>

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
         * Write the specified buffer out to the channel
         *
         * @param buff the data to write
         * @param len the number of bytes in the buffer
         */
        virtual void write(const char* buff, size_t len) = 0;
        /**
         * Read the specified number of bytes from the channel into the buffer.
         *
         * @param buff The buffer to place the bytes into
         * @param count The number of bytes to read
         * @return the actual number of bytes read from the channel
         */
        virtual size_t read(char* &buff, size_t count) = 0;

        /**
         * Write the buffer to the physical device
         *
         * @return the number of bytes written
         */
        virtual uint32_t flush() = 0;
    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_IPHYSICALLAYER_H
