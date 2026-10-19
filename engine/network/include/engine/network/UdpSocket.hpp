#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include "engine/network/Export.hpp"
#include "engine/network/Address.hpp"

namespace Engine {
    struct Datagram {
        Address from;
        std::vector<std::uint8_t> bytes;
    };

    class ENGINE_NETWORK_EXPORT UdpSocket
    {
    public:
        explicit UdpSocket(std::uint16_t port = 0);
        ~UdpSocket();

        UdpSocket(const UdpSocket &) = delete;
        UdpSocket &operator=(const UdpSocket &) = delete;

        std::uint16_t port() const;
        void send(const Address &to, std::vector<std::uint8_t> bytes);
        std::vector<Datagram> receive();

        static Address resolve(const std::string &host, std::uint16_t port);
    private:
        struct Impl;
        std::unique_ptr<Impl> _impl;
    };
}
