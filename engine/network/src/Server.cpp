#include <utility>
#include <algorithm>
#include <stdexcept>
#include "engine/network/Server.hpp"
#include "engine/network/BinaryWriter.hpp"

namespace Engine {
    Server::Server(std::uint16_t port, std::size_t maxClients, std::uint8_t version)
        : _socket(port), _maxClients(maxClients), _version(version)
    {
        if (maxClients == 0) {
            throw std::invalid_argument("Server: maxClients must be positive");
        }
    }

    Server::~Server()
    {
        for (Session &session : _sessions) {
            sendDisconnect(session, DisconnectReason::Shutdown);
        }
    }

    std::uint16_t Server::port() const
    {
        return _socket.port();
    }

    std::size_t Server::clientCount() const
    {
        return _sessions.size();
    }

    std::optional<std::chrono::milliseconds> Server::latency(ClientId client)
    {
        Session *session = find(client);

        if (session == nullptr || !session->connection.latency()) {
            return std::nullopt;
        }
        return std::chrono::duration_cast<std::chrono::milliseconds>(*session->connection.latency());
    }

    void Server::update()
    {
        Clock::time_point now = Clock::now();

        for (const Datagram &datagram : _socket.receive()) {
            handleDatagram(datagram, now);
        }
        checkSessions(now);
    }

    std::optional<ServerEvent> Server::pollEvent()
    {
        std::optional<ServerEvent> event;

        if (_events.empty()) {
            return std::nullopt;
        }
        event = std::move(_events.front());
        _events.pop_front();
        return event;
    }

    void Server::send(ClientId client, MessageType type, std::vector<std::uint8_t> payload, Delivery delivery)
    {
        Session *session = find(client);

        if (session != nullptr) {
            session->connection.send(type, std::move(payload), delivery, Clock::now());
        }
    }

    void Server::broadcast(MessageType type, const std::vector<std::uint8_t> &payload, Delivery delivery)
    {
        Clock::time_point now = Clock::now();

        for (Session &session : _sessions) {
            session.connection.send(type, payload, delivery, now);
        }
    }

    void Server::disconnect(ClientId client)
    {
        kick(client);
    }

    void Server::flush()
    {
        for (Session &session : _sessions) {
            flush(session);
        }
    }

    Server::Session *Server::find(ClientId id)
    {
        auto found = std::find_if(_sessions.begin(), _sessions.end(), [id](const Session &session) {
            return session.id == id;
        });

        if (found == _sessions.end()) {
            return nullptr;
        }
        return &*found;
    }

    Server::Session *Server::findByToken(std::uint32_t token)
    {
        auto found = std::find_if(_sessions.begin(), _sessions.end(), [token](const Session &session) {
            return session.connection.token() == token;
        });

        if (found == _sessions.end()) {
            return nullptr;
        }
        return &*found;
    }

    Server::Session *Server::findByAddress(const Address &address)
    {
        auto found = std::find_if(_sessions.begin(), _sessions.end(), [&address](const Session &session) {
            return session.address == address;
        });

        if (found == _sessions.end()) {
            return nullptr;
        }
        return &*found;
    }

    void Server::handleDatagram(const Datagram &datagram, Clock::time_point now)
    {
        std::optional<Packet> packet = Packet::parse(datagram.bytes);
        Session *session;

        if (!packet) {
            return;
        }
        if (packet->isHandshake(Protocol::CONNECT)) {
            handleConnect(datagram.from, *packet, now);
            return;
        }
        session = findByToken(packet->token());
        if (session != nullptr && session->address == datagram.from) {
            handleSession(*session, *packet, now);
        }
    }

    void Server::handleConnect(const Address &from, const Packet &packet, Clock::time_point now)
    {
        Session *session = findByAddress(from);
        std::optional<RefuseReason> refusal;

        if (session != nullptr) {
            sendAccept(*session);
            return;
        }
        refusal = admit(packet.messages()[0].payload[0]);
        if (refusal) {
            sendRefuse(from, *refusal);
            return;
        }
        open(from, now);
    }

    void Server::handleSession(Session &session, const Packet &packet, Clock::time_point now)
    {
        ClientId id = session.id;

        if (!session.connection.receive(packet, now)) {
            return;
        }
        for (Message &message : session.connection.takeMessages()) {
            _events.push_back(ServerEvents::Message{id, message.type, std::move(message.payload)});
        }
        if (session.connection.peerDisconnect()) {
            close(id, *session.connection.peerDisconnect());
        }
    }

    std::optional<RefuseReason> Server::admit(std::uint8_t clientVersion) const
    {
        if (clientVersion != _version) {
            return RefuseReason::Version;
        }
        if (_sessions.size() >= _maxClients) {
            return RefuseReason::Full;
        }
        return std::nullopt;
    }

    void Server::open(const Address &from, Clock::time_point now)
    {
        _sessions.push_back({_nextClientId, from, Connection(makeToken(), now)});
        ++_nextClientId;
        sendAccept(_sessions.back());
        _events.push_back(ServerEvents::Connected{_sessions.back().id});
    }

    void Server::checkSessions(Clock::time_point now)
    {
        std::vector<ClientId> timedOut;
        std::vector<ClientId> broken;

        for (Session &session : _sessions) {
            session.connection.update(now);
            if (session.connection.timedOut(now)) {
                timedOut.push_back(session.id);
            } else if (session.connection.broken()) {
                broken.push_back(session.id);
            }
        }
        for (ClientId id : timedOut) {
            close(id, DisconnectReason::Timeout);
        }
        for (ClientId id : broken) {
            kick(id);
        }
    }

    void Server::sendAccept(Session &session)
    {
        Packet packet(session.connection.nextSequence(), 0);
        BinaryWriter payload;

        payload.writeU32(session.connection.token());
        packet.add({Protocol::ACCEPT, false, 0, payload.data()});
        _socket.send(session.address, packet.serialize());
    }

    void Server::sendRefuse(const Address &to, RefuseReason reason)
    {
        Packet packet(0, 0);

        packet.add({Protocol::REFUSE, false, 0, {static_cast<std::uint8_t>(reason)}});
        _socket.send(to, packet.serialize());
    }

    void Server::sendDisconnect(Session &session, DisconnectReason reason)
    {
        for (std::size_t i = 0; i < Protocol::DISCONNECT_COPIES; ++i) {
            session.connection.sendEngine(Protocol::DISCONNECT, {static_cast<std::uint8_t>(reason)});
            flush(session);
        }
    }

    void Server::flush(Session &session)
    {
        for (const Packet &packet : session.connection.flush()) {
            _socket.send(session.address, packet.serialize());
        }
    }

    void Server::kick(ClientId id)
    {
        Session *session = find(id);

        if (session == nullptr) {
            return;
        }
        sendDisconnect(*session, DisconnectReason::Kicked);
        close(id, DisconnectReason::Kicked);
    }

    void Server::close(ClientId id, DisconnectReason reason)
    {
        std::erase_if(_sessions, [id](const Session &session) {
            return session.id == id;
        });
        _events.push_back(ServerEvents::Disconnected{id, reason});
    }

    std::uint32_t Server::makeToken()
    {
        std::uint32_t token = 0;

        while (token == 0 || findByToken(token) != nullptr) {
            token = static_cast<std::uint32_t>(_random());
        }
        return token;
    }
}
