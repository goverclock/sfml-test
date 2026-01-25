#pragma once
#include <SFML/Network.hpp>

namespace lan {
namespace packet {

struct Heartbeat {
    sf::IpAddress from = sf::IpAddress::Broadcast;
    sf::IpAddress to = sf::IpAddress::Broadcast;
};

sf::Packet& operator<<(sf::Packet& packet, const Heartbeat& heartbeat);
sf::Packet& operator>>(sf::Packet& packet, Heartbeat& heartbeat);

struct GuestsInRoom {
    std::string nickname;
    int signal_strength;
};

sf::Packet& operator<<(sf::Packet& packet, const GuestsInRoom& gir);
sf::Packet& operator>>(sf::Packet& packet, GuestsInRoom& gir);

};  // namespace packet
};  // namespace lan
