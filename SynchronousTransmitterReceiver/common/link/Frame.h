//
// Created by nathan on 9/21/17.
//

#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_FRAME_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_FRAME_H

#include <cstdlib>

namespace libsts::link
{
    struct Frame {
        size_t Length;
        char*  Payload;
    };
}



#endif //SYNCHRONOUSTRANSMITTERRECEIVER_FRAME_H
