#pragma once
#include <SFML/Network.hpp>
#include <ctime>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

#include "ergonomics.hpp"

namespace lan {

enum class SignalStrength {
    Weak,
    Medium,
    Strong,
    Lost,
};

class GuestConnection : public sf::TcpSocket {
   public:
    GuestConnection() { UNREACHABLE(); };
    GuestConnection(sf::TcpSocket&& socket)
        : sf::TcpSocket(std::move(socket)),
          mLastHeartbeat(std::time(nullptr)) {};

    bool check_update() const;
    std::string guest_ip() const;
    static constexpr int guest_lost_timeout = 5;
    SignalStrength signal_strength() const;

    std::time_t mLastHeartbeat;

   private:
    // record return value of last signal_strength() call, to calculate updates
    SignalStrength mLastSignalStrengthl = SignalStrength::Strong;
};

class GuestConnectionManager {
   public:
    GuestConnectionManager() : mBookkeepThread([&]() { bookkeep(); }) {};
    ~GuestConnectionManager();

    void add_connection(sf::TcpSocket&& socket);  // TODO: assert no duplication
    // no need to remove connection method, beacause it's already handled inside
    // this class
    void disconnect_all();
    int guest_count();
    bool check_guest_info_update();
    const std::unordered_map<std::string, GuestConnection>&
    guest_connection_list();

   private:
    void bookkeep();

    std::mutex mLock;
    bool mIsDestructed = false;
    bool mIsAnyConnErased = false;
    std::unordered_map<std::string, GuestConnection> mConnections;
    sf::SocketSelector mConnectionSelector;
    std::thread mBookkeepThread;
};

};  // namespace lan
