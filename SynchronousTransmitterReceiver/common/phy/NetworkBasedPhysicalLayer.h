#ifndef SYNCHRONOUSTRANSMITTERRECEIVER_NETWORKBASEDPHYSICALLAYER_H
#define SYNCHRONOUSTRANSMITTERRECEIVER_NETWORKBASEDPHYSICALLAYER_H

#include <cstddef>
#include <iostream>

#include "IPhysicalLayer.h"
#include "PhysicalLayerException.h"
#include <unistd.h>

namespace libsts::phy
{
    class NetworkBasedPhysicalLayer : public IPhysicalLayer {
    public:
        /**
         * Create a network-based phy that listens on the specified port
         *
         * @param port the port to listen on
         */
        explicit NetworkBasedPhysicalLayer(uint16_t port)
                : port(port)
        {
        }

        /**
         * Create a network-based phy that connects to the specified remote
         *
         * @param remote the host or IP to connect to
         * @param port the port to connect on
         */
        NetworkBasedPhysicalLayer(std::string remote, uint16_t port)
            : remote(std::move(remote)), port(port)
        {
        }

        ~NetworkBasedPhysicalLayer() override {
            close();
        }

        void waitForConnection();

        bool isClientPhy() { return remote.empty(); }
        bool isServerPhy() { return !isClientPhy(); }
        bool eof() override;
        Direction getDirection() override;

    public:
        void close() override;

        void open() override;

        void write(const char *data, size_t len) override;

        size_t read(char *buff, size_t len) override;

        uint32_t flush() override;

    private:
        int mySock = -1, listenSock = -1;
        std::string remote = "";
        uint16_t port{};

        void open_client();
        void open_server();
    };
}


#endif //SYNCHRONOUSTRANSMITTERRECEIVER_NETWORKBASEDPHYSICALLAYER_H
