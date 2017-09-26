//
// Created by nathan on 9/21/17.
//

#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H


#include <memory>
#include "../phy/IPhysicalLayer.h"
#include "Frame.h"

/**
 * An abstraction representing the link layer in the OSI Network Model
 */
namespace libsts::link
{
    class LinkLayer {
    public:
        explicit LinkLayer(std::shared_ptr<libsts::phy::IPhysicalLayer> &phy)
            : phy(phy)
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
         * Write a frame to the link
         * @param frame The frame to write
         */
        void write(std::shared_ptr<Frame> frame);
        /**
         * Read a frame from the link
         * @return a frame read from the link
         */
        std::shared_ptr<Frame> read();
    private:
        std::shared_ptr<libsts::phy::IPhysicalLayer> phy;
        bool closed = true;
    };
}


#endif //SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYER_H
