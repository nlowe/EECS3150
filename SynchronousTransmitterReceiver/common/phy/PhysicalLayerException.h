#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_PHYSICALLAYEREXCEPTION_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_PHYSICALLAYEREXCEPTION_H

#include <stdexcept>

namespace libsts::phy
{
    /**
     * An exception thrown by the physical layer
     */
    class PhysicalLayerException: public std::runtime_error
    {
    public:
        explicit PhysicalLayerException(const std::string &__arg) : runtime_error(__arg) {}
    };

    /**
     * An exception thrown when the phy fails to bind to the specified port
     */
    class BindException: public PhysicalLayerException
    {
    public:
        explicit BindException(const std::string &__arg) : PhysicalLayerException(__arg) {}
    };

    /**
     * An exception thrown when the phy encounters an unknown IOException
     */
    class IOException: public PhysicalLayerException
    {
    public:
        explicit IOException(const std::string &__arg) : PhysicalLayerException(__arg) {}
    };

    /**
     * A bad encoding was passed to the physical layer
     */
    class IllegalEncodingException: public PhysicalLayerException
    {
    public:
        explicit IllegalEncodingException(const std::string &__arg) : PhysicalLayerException(__arg) {}
    };

    /**
     * The parity check failed
     */
    class ParityCheckFailure: public PhysicalLayerException
    {
    public:
        explicit ParityCheckFailure(const std::string &__arg) : PhysicalLayerException(__arg) {}
    };

    /**
     * The channel is not onnected
     */
    class NotConnectedException: public PhysicalLayerException
    {
    public:
        explicit NotConnectedException(const std::string &__arg) : PhysicalLayerException(__arg) {}
    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_PHYSICALLAYEREXCEPTION_H
