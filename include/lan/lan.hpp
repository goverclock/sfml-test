#pragma once
#include <SFML/Network.hpp>
#include <atomic>
#include <cassert>
#include <ctime>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "ergonomics.hpp"
#include "guest_connection.hpp"

namespace lan {

enum class LanMessageUpdated { RoomInfoList, GuestInRoom, HostDismissRoom };

// TODO: actually room info
struct RoomInfo {
    std::string ip;  // port is unneeded since it's fixed to LanPeer::PORT
    SignalStrength signal_strength;

    bool operator==(const RoomInfo& other) const {
        return ip == other.ip && signal_strength == other.signal_strength;
    }
    std::string to_string() {
        std::string ret = ip + ":";
        switch (signal_strength) {
            case SignalStrength::Strong:
                ret += "Strong";
                break;
            case SignalStrength::Medium:
                ret += "Medium";
                break;
            case SignalStrength::Weak:
                ret += "Weak";
                break;
            default:
                UNREACHABLE();
        }
        return ret;
    }
};

class LanPeer {
   public:
    LanPeer() = default;
    ~LanPeer() = default;

    // as host
    void start_periodically_broadcast();
    void stop_periodically_broadcast();
    void start_listen_guest();
    void stop_listen_guest();
    void disconnect_all_guests();
    const std::unordered_map<std::string, GuestConnection>&
    get_connected_guest_info_list();

    // as guest
    void start_periodically_discover();
    void stop_periodically_discover();
    bool connect_to_host(std::string host_ip);
    void disconnect_from_host();
    void start_heartbeat_to_host();
    void stop_heartbeat_to_host();

    std::optional<LanMessageUpdated> poll_updates();
    std::vector<RoomInfo> get_room_info_list();

   private:
    constexpr static int PORT = 6969;
    constexpr static int BROADCAST_INTERVAL = 1;  // seconds

    std::mutex mLock;
    void enque_updateL(LanMessageUpdated message);
    std::unordered_set<LanMessageUpdated> mUpdatedSet;

    void update_room_info();
    // TODO: change to sf::IpAddr, RoomInfo
    std::map<std::string, RoomInfo> mRoomInfoList;
    // TODO: change to sf::IpAddr, std::time_t
    std::unordered_map<std::string, std::time_t> mRoomLastHeard;

    sf::TcpSocket mToHostTcpSocket;
    void update_connected_guest_info();
    GuestConnectionManager mGuestConnectionManager;

    std::atomic<bool> mIsBroadcasting = false;
    std::atomic<bool> mIsListeningGuest = false;
    std::atomic<bool> mIsDiscovering = false;
    std::atomic<bool> mIsHeartbeating = false;
};

};  // namespace lan
