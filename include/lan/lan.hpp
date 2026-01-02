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

namespace lan {

enum class LanMessageUpdated { PeerInfoList, GuestInRoom };

enum class SignalStrength {
    Weak,
    Medium,
    Strong,
};

// TODO: actually room info
struct PeerInfo {
    std::string ip;  // port is unneeded since it's fixed to LanPeer::PORT
    SignalStrength signal_strength;

    bool operator==(const PeerInfo& other) const {
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

struct ConnectedGuestInfo {
    std::string guest_ip;
    SignalStrength signal_strength;
    bool operator==(const ConnectedGuestInfo& other) const {
        return guest_ip == other.guest_ip &&
               signal_strength == other.signal_strength;
    }
    std::string to_string() {
        std::string ret = "Guest: " + guest_ip;
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

    // as guest
    void start_periodically_discover();
    void stop_periodically_discover();
    bool connect_to_host(std::string host_ip);
    void disconnect_from_host();
    void start_heartbeat_to_host();
    void stop_heartbeat_to_host();

    std::optional<LanMessageUpdated> poll_updates();
    std::vector<PeerInfo> get_peer_info_list();
    std::vector<ConnectedGuestInfo> get_connected_guest_info_list();

   private:
    constexpr static int PORT = 6969;
    constexpr static int BROADCAST_INTERVAL = 1;  // seconds

    std::mutex mLock;
    void enque_updateL(LanMessageUpdated message);
    std::unordered_set<LanMessageUpdated> mUpdatedSet;

    // TODO: actually just room info, change these names
    void update_peer_info();
    // TODO: change to sf::IpAddr, PeerInfo
    std::map<std::string, PeerInfo> mPeerInfoList;
    // TODO: change to sf::IpAddr, std::time_t
    std::unordered_map<std::string, std::time_t> mRoomLastHeard;

    sf::TcpSocket mToHostTcpSocket;
    void update_connected_guest_info();
    std::map<std::string, ConnectedGuestInfo> mConnectedGuestInfo;
    std::unordered_map<std::string, sf::TcpSocket> mGuestConnections;
    std::unordered_map<std::string, std::time_t> mGuestLastHeatbeat;

    std::atomic<bool> mIsBroadcasting = false;
    std::atomic<bool> mIsListeningGuest = false;
    std::atomic<bool> mIsDiscovering = false;
    std::atomic<bool> mIsHeartbeating = false;
};

};  // namespace lan
