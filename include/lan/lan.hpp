#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

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

namespace lan {

enum class LanMessageUpdated { PeerInfoList };

struct PeerInfo {
    std::string ip;  // port is unneeded since it's fixed to LanPeer::PORT
    enum class SignalStrength {
        Weak,
        Medium,
        Strong,
    } signal_strength;

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
                assert(false && "not implemented");
        }
        return ret;
    }
};

class LanPeer {
   public:
    LanPeer();
    ~LanPeer();

    // as host
    void start_periodically_broadcast();
    void stop_periodically_broadcast();
    void start_listen_guest();
    void stop_listen_guest();

    // as guest
    void start_periodically_discover();
    void stop_periodically_discover();
    bool connect_to_host(std::string host_ip);
    void start_heartbeat_to_host();

    std::optional<LanMessageUpdated> poll_updates();
    std::vector<PeerInfo> get_peer_info_list();

   private:
    std::mutex mLock;

    void update_peer_info();
    std::map<std::string, PeerInfo> mPeerInfoList;
    std::unordered_map<std::string, std::time_t> mLastHeard;

    void enque_updateL(LanMessageUpdated message);
    std::unordered_set<LanMessageUpdated> mUpdatedSet;

    // host
    void broadcast_as_host();

    // guest
    void receive_as_guest();
    sf::TcpSocket mTcpSocket;

    constexpr static int PORT = 6969;
    constexpr static int BROADCAST_INTERVAL = 1;  // seconds
    int mSocketFd;
    struct sockaddr_in mBroadcastAddr;
    std::atomic<bool> mIsBroadcasting = false;
    std::atomic<bool> mIsListeningGuest = false;
    std::atomic<bool> mIsDiscovering = false;
};

};  // namespace lan
