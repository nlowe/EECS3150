#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_ERRORCORRECTIONTYPE_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_ERRORCORRECTIONTYPE_H

namespace libsts::link
{
    /**
     * The type of error correction used
     */
    enum class ErrorCorrectionType
    {
        /**
         * Indicates that the link is using CRC-16-ANSI for error detection
         */
        CRC_ANSI_16,
        /**
         * Indicates that the link is using double hamming-7-4 for error correction
         */
        HAMMING
    };
}

#endif //SYNCHRONOUSTRANSMITTERRECEIVER_ERRORCORRECTIONTYPE_H
