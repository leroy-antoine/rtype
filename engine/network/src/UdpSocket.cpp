#include <utility>
#include "UdpSocketImpl.hpp"
#include "engine/network/Protocol.hpp"

namespace Engine {
    namespace {
        asio::ip::udp::endpoint toEndpoint(const Address &address)
        {
            return {asio::ip::address_v4(address.ip), address.port};
        }

        Address toAddress(const asio::ip::udp::endpoint &endpoint)
        {
            return {endpoint.address().to_v4().to_uint(), endpoint.port()};
        }
    }

    UdpSocket::Impl::Impl(std::uint16_t port)
        : socket(io, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)), localPort(socket.local_endpoint().port())
    {
    }

    void UdpSocket::Impl::startReceive()
    {
        socket.async_receive_from(asio::buffer(buffer), sender, [this](const asio::error_code &error, std::size_t size) {
            if (error == asio::error::operation_aborted || !socket.is_open()) {
                return;
            }
            if (!error) {
                store(size);
            }
            startReceive();
        });
    }

    void UdpSocket::Impl::store(std::size_t size)
    {
        std::lock_guard lock(mutex);

        if (size > Protocol::MAX_PACKET_SIZE || received.size() >= MAX_QUEUED) {
            return;
        }
        received.push_back({toAddress(sender), {buffer.begin(), buffer.begin() + static_cast<std::ptrdiff_t>(size)}});
    }

    UdpSocket::UdpSocket(std::uint16_t port) : _impl(std::make_unique<Impl>(port))
    {
        _impl->startReceive();
        _impl->thread = std::thread([this] {
            _impl->io.run();
        });
    }

    UdpSocket::~UdpSocket()
    {
        asio::post(_impl->io, [this] {
            asio::error_code error;

            _impl->socket.close(error);
        });
        _impl->thread.join();
    }

    std::uint16_t UdpSocket::port() const
    {
        return _impl->localPort;
    }

    void UdpSocket::send(const Address &to, std::vector<std::uint8_t> bytes)
    {
        asio::post(_impl->io, [this, to, bytes = std::move(bytes)] {
            asio::error_code error;

            _impl->socket.send_to(asio::buffer(bytes), toEndpoint(to), 0, error);
        });
    }

    std::vector<Datagram> UdpSocket::receive()
    {
        std::lock_guard lock(_impl->mutex);
        std::vector<Datagram> datagrams = std::move(_impl->received);

        _impl->received.clear();
        return datagrams;
    }

    Address UdpSocket::resolve(const std::string &host, std::uint16_t port)
    {
        asio::io_context io;
        asio::ip::udp::resolver resolver(io);

        return toAddress(*resolver.resolve(asio::ip::udp::v4(), host, std::to_string(port)).begin());
    }
}
