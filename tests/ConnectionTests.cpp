#include <cstdint>
#include <vector>
#include "engine/network/Connection.hpp"
#include "engine/network/Packet.hpp"
#include "TestHarness.hpp"

namespace {
    constexpr Engine::MessageType USER = Engine::Protocol::FIRST_USER_MESSAGE;

    Engine::Packet packet(const std::uint16_t sequence, const std::vector<Engine::Message> &messages)
    {
        Engine::Packet result(sequence, 1);

        for (const Engine::Message &message : messages) {
            result.add(message);
        }
        return result;
    }

    Engine::Message unreliable(const std::uint8_t value)
    {
        return Engine::Message{USER, false, 0, {value}};
    }

    Engine::Message reliable(const std::uint16_t id, const std::uint8_t value)
    {
        return Engine::Message{USER, true, id, {value}};
    }

    std::vector<std::uint8_t> values(Engine::Connection &connection)
    {
        std::vector<std::uint8_t> result;

        for (const Engine::Message &message : connection.takeMessages()) {
            result.push_back(message.payload[0]);
        }
        return result;
    }

    bool sendsAck(Engine::Connection &connection)
    {
        for (const Engine::Packet &sent : connection.flush()) {
            for (const Engine::Message &message : sent.messages()) {
                if (message.type == Engine::Protocol::ACK) {
                    return true;
                }
            }
        }
        return false;
    }
}

TEST(a_late_packet_still_delivers_its_reliable_messages)
{
    const auto now = Engine::Clock::now();
    Engine::Connection connection(1, now);

    CHECK(connection.receive(packet(2, {reliable(1, 20)}), now));
    CHECK(values(connection).empty());
    CHECK(connection.receive(packet(1, {reliable(0, 10)}), now));
    CHECK((values(connection) == std::vector<std::uint8_t>{10, 20}));
}

TEST(a_late_packet_drops_its_unreliable_messages)
{
    const auto now = Engine::Clock::now();
    Engine::Connection connection(1, now);

    connection.receive(packet(5, {unreliable(5)}), now);
    CHECK((values(connection) == std::vector<std::uint8_t>{5}));
    connection.receive(packet(4, {unreliable(4), reliable(0, 40)}), now);
    CHECK((values(connection) == std::vector<std::uint8_t>{40}));
    connection.receive(packet(6, {unreliable(6)}), now);
    CHECK((values(connection) == std::vector<std::uint8_t>{6}));
}

TEST(a_late_packet_is_still_acknowledged)
{
    const auto now = Engine::Clock::now();
    Engine::Connection connection(1, now);

    connection.receive(packet(9, {unreliable(9)}), now);
    connection.flush();
    connection.receive(packet(8, {reliable(0, 80)}), now);
    CHECK(sendsAck(connection));
}

TEST(a_late_ack_stops_the_resend)
{
    auto now = Engine::Clock::now();
    Engine::Connection connection(1, now);

    connection.send(USER, {1}, Engine::Delivery::Reliable, now);
    connection.flush();
    connection.receive(packet(3, {unreliable(3)}), now);
    connection.receive(packet(2, {Engine::Message{Engine::Protocol::ACK, false, 0, {0, 0}}}), now);
    now += Engine::Protocol::TIMEOUT / 2;
    connection.update(now);
    for (const Engine::Packet &sent : connection.flush()) {
        for (const Engine::Message &message : sent.messages()) {
            CHECK(message.type != USER);
        }
    }
}

TEST(a_duplicated_packet_delivers_its_reliable_message_once)
{
    const auto now = Engine::Clock::now();
    Engine::Connection connection(1, now);
    const Engine::Packet twice = packet(1, {reliable(0, 7), unreliable(8)});

    connection.receive(twice, now);
    CHECK((values(connection) == std::vector<std::uint8_t>{7, 8}));
    connection.receive(twice, now);
    CHECK(values(connection).empty());
}
