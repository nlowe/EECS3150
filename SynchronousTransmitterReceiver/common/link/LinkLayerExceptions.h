//
// Created by nathan on 10/3/17.
//

#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYEREXCEPTIONS_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYEREXCEPTIONS_H

#include <stdexcept>

namespace libsts::link
{
    class LinkException : public std::runtime_error
    {
    public:
        explicit LinkException(const std::string &__arg) : runtime_error(__arg) {}

    };

    class FramingException : public LinkException
    {
    public:
        explicit FramingException(const std::string &__arg) : LinkException(__arg) {}

    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYEREXCEPTIONS_H
