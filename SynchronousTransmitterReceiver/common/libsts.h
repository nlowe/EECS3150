#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_H

namespace libsts
{
    /**
     * The direction a channel is open in
     */
    enum class Direction
    {
        /**
         * The channel is open for read
         */
        READ,
        /**
         * The channel is open for write
         */
        WRITE
    };

    /**
     * Signifies that the channel was not opened in the direction the operation was attempted
     */
    class BadDirectionException : public std::runtime_error
    {
    public:
        explicit BadDirectionException(const std::string &__arg) : runtime_error(__arg) {}

    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_H
