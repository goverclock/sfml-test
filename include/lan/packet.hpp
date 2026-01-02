#pragma once
#include <SFML/Network.hpp>

namespace lan {

namespace packet {

struct Heartbeat {
    sf::IpAddress from = sf::IpAddress::Broadcast;
    sf::IpAddress to = sf::IpAddress::Broadcast;
};

sf::Packet& operator<<(sf::Packet& packet, const Heartbeat& heartbeat) {
    return packet << heartbeat.from.toString() << heartbeat.to.toString();
}

sf::Packet& operator>>(sf::Packet& packet, Heartbeat& heartbeat) {
    std::string from, to;
    packet >> from >> to;
    heartbeat.from = *sf::IpAddress::resolve(from);
    heartbeat.to = *sf::IpAddress::resolve(to);
    return packet;
}

};  // namespace packet

};  // namespace lan
