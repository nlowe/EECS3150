#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_H

namespace libsts
{
    enum class Direction
    {
        READ,
        WRITE
    };

    class BadDirectionException : public std::runtime_error
    {
    public:
        explicit BadDirectionException(const std::string &__arg) : runtime_error(__arg) {}

    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_H
