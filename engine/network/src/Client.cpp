#include <utility>
#include <stdexcept>
#include "engine/network/Client.hpp"
#include "engine/network/BinaryReader.hpp"

namespace Engine {
    Client::Client(const std::string &host, std::uint16_t port, std::uint8_t version)
        : _server(UdpSocket::resolve(host, port)), _version(version)
    {
    }

    Client::~Client()
    {
        disconnect();
    }

    bool Client::connected() const
    {
        return _state == State::Connected;
    }

    std::optional<std::chrono::milliseconds> Client::latency() const
    {
        if (!_connection || !_connection->latency()) {
            return std::nullopt;
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(*_connection->latency());
    }

    void Client::update()
    {
        Clock::time_point now = Clock::now();

        for (const Datagram &datagram : _socket.receive()) {
            handleDatagram(datagram, now);
        }
        switch (_state) {
            case State::Connecting:
                tryConnect(now);
                break;
            case State::Connected:
                checkConnection(now);
                break;
            case State::Closed:
                break;
        }
    }

    std::optional<ClientEvent> Client::pollEvent()
    {
        std::optional<ClientEvent> event;

        if (_events.empty()) {
            return std::nullopt;
        }
        event = std::move(_events.front());
        _events.pop_front();
        return event;
    }

    void Client::send(MessageType type, std::vector<std::uint8_t> payload, Delivery delivery)
    {
        if (_state != State::Connected) {
            throw std::logic_error("send: client is not connected");
        }
        _connection->send(type, std::move(payload), delivery, Clock::now());
    }

    void Client::disconnect()
    {
        if (_state == State::Connected) {
            sendDisconnect(DisconnectReason::Quit);
            _connection.reset();
        }
        _state = State::Closed;
    }

    void Client::flush()
    {
        if (_state == State::Connected) {
            flushConnection();
        }
    }

    void Client::handleDatagram(const Datagram &datagram, Clock::time_point now)
    {
        std::optional<Packet> packet = Packet::parse(datagram.bytes);

        if (!packet || datagram.from != _server) {
            return;
        }
        switch (_state) {
            case State::Connecting:
                handleHandshake(*packet, now);
                break;
            case State::Connected:
                handleSession(*packet, now);
                break;
            case State::Closed:
                break;
        }
    }

    void Client::handleHandshake(const Packet &packet, Clock::time_point now)
    {
        BinaryReader reader(packet.messages()[0].payload);
        std::optional<std::uint32_t> token;

        if (packet.isHandshake(Protocol::REFUSE)) {
            _state = State::Closed;
            _events.push_back(ClientEvents::Refused{static_cast<RefuseReason>(packet.messages()[0].payload[0])});
            return;
        }
        if (!packet.isHandshake(Protocol::ACCEPT)) {
            return;
        }
        token = reader.readU32();
        if (!token || *token == 0) {
            return;
        }
        _connection.emplace(*token, now);
        _state = State::Connected;
        _events.push_back(ClientEvents::Connected{});
    }

    void Client::handleSession(const Packet &packet, Clock::time_point now)
    {
        if (packet.token() != _connection->token() || !_connection->receive(packet, now)) {
            return;
        }
        for (Message &message : _connection->takeMessages()) {
            _events.push_back(ClientEvents::Message{message.type, std::move(message.payload)});
        }
        if (_connection->peerDisconnect()) {
            close(*_connection->peerDisconnect());
        }
    }

    void Client::tryConnect(Clock::time_point now)
    {
        Packet packet(_handshakeSequence, 0);

        if (_attempts != 0 && now - _lastAttempt < Protocol::CONNECT_RETRY_DELAY) {
            return;
        }
        if (_attempts == Protocol::MAX_CONNECT_ATTEMPTS) {
            _state = State::Closed;
            _events.push_back(ClientEvents::Unreachable{});
            return;
        }
        packet.add({Protocol::CONNECT, false, 0, {_version}});
        _socket.send(_server, packet.serialize());
        ++_handshakeSequence;
        ++_attempts;
        _lastAttempt = now;
    }

    void Client::checkConnection(Clock::time_point now)
    {
        _connection->update(now);
        if (_connection->broken()) {
            sendDisconnect(DisconnectReason::Quit);
            close(DisconnectReason::Timeout);
            return;
        }
        if (_connection->timedOut(now)) {
            close(DisconnectReason::Timeout);
        }
    }

    void Client::sendDisconnect(DisconnectReason reason)
    {
        for (std::size_t i = 0; i < Protocol::DISCONNECT_COPIES; ++i) {
            _connection->sendEngine(Protocol::DISCONNECT, {static_cast<std::uint8_t>(reason)});
            flushConnection();
        }
    }

    void Client::flushConnection()
    {
        for (const Packet &packet : _connection->flush()) {
            _socket.send(_server, packet.serialize());
        }
    }

    void Client::close(DisconnectReason reason)
    {
        _state = State::Closed;
        _connection.reset();
        _events.push_back(ClientEvents::Disconnected{reason});
    }
}
