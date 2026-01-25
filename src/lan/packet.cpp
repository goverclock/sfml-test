#include "lan/packet.hpp"

namespace lan {
namespace packet {

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

sf::Packet& operator<<(sf::Packet& packet, const GuestsInRoom& gir) {
    return packet << gir.nickname << gir.signal_strength;
}

sf::Packet& operator>>(sf::Packet& packet, GuestsInRoom& gir) {
    packet >> gir.nickname >> gir.signal_strength;
    return packet;
}

};  // namespace packet
};  // namespace lan
