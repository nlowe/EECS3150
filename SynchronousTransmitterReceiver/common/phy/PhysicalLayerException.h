//
// Created by nathan on 9/21/17.
//

#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_PHYSICALLAYEREXCEPTION_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_PHYSICALLAYEREXCEPTION_H

#include <stdexcept>

namespace libsts::phy
{
    class PhysicalLayerException: public std::runtime_error
    {
    public:
        explicit PhysicalLayerException(const std::string &__arg) : runtime_error(__arg) {}
    };

    class NotConnectedException: public PhysicalLayerException
    {
    public:
        explicit NotConnectedException(const std::string &__arg) : PhysicalLayerException(__arg) {}
    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_PHYSICALLAYEREXCEPTION_H
