//
// Created by nathan on 9/21/17.
//

#include "LinkLayer.h"

namespace libsts::link
{

    void LinkLayer::open() {
        if(!closed) return;

        phy->open();
        closed = false;
    }

    void LinkLayer::close() {
        if(closed) return;

        phy->close();
        closed = true;
    }

    void LinkLayer::write(std::shared_ptr<Frame> frame) {

    }

    std::shared_ptr<Frame> LinkLayer::read() {
        return std::shared_ptr<Frame>();
    }
}