#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYEREXCEPTIONS_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYEREXCEPTIONS_H

#include <stdexcept>

namespace libsts::link
{
    /**
     * An exception thrown in the link layer
     */
    class LinkException : public std::runtime_error
    {
    public:
        explicit LinkException(const std::string &__arg) : runtime_error(__arg) {}

    };

    /**
     * An exeption thrown during the framing or de-framing process
     */
    class FramingException : public LinkException
    {
    public:
        explicit FramingException(const std::string &__arg) : LinkException(__arg) {}

    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_LINKLAYEREXCEPTIONS_H
