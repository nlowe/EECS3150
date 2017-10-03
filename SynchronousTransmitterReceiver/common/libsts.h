#ifndef COMMON_LIBRARY_H
#define COMMON_LIBRARY_H

void hello();

namespace libsts
{
    /**
     * The direction a communication channel is operating in
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
}

#endif