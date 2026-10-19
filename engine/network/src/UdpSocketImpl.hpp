#pragma once

#include <array>
#include <mutex>
#include <thread>
#include <vector>
#include <cstdint>
#include <cstddef>
#include <asio.hpp>
#include "engine/network/UdpSocket.hpp"

namespace Engine {
    struct UdpSocket::Impl {
        static constexpr std::size_t BUFFER_SIZE = 2048;
        static constexpr std::size_t MAX_QUEUED = 4096;

        explicit Impl(std::uint16_t port);

        void startReceive();
        void store(std::size_t size);

        asio::io_context io;
        asio::ip::udp::socket socket;
        std::uint16_t localPort;
        asio::ip::udp::endpoint sender;
        std::array<std::uint8_t, BUFFER_SIZE> buffer{};
        std::mutex mutex;
        std::vector<Datagram> received;
        std::thread thread;
    };
}
