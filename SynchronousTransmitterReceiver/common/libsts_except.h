//
// Created by nathan on 10/1/17.
//

#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_EXCEPT_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_EXCEPT_H

#include <stdexcept>

namespace libsts
{
    /**
     * Indicates an error occurred in the usage of the link that conflicted with the direction
     * the channel or link is allowed to operate in.
     */
    class LinkDirectionException: public std::runtime_error
    {
        public:
        explicit LinkDirectionException(const std::string &__arg) : runtime_error(__arg) {}
    };

}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_LIBSTS_EXCEPT_H
