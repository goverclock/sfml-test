#include <fcntl.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <print>
#include <thread>

#include "ergonomics.hpp"
#include "lan/lan.hpp"

namespace lan {

LanPeer::LanPeer() {
    mSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(mSocketFd);
    int val = 1;
    int ret =
        setsockopt(mSocketFd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
    assert(!ret);
    ret = setsockopt(mSocketFd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    assert(!ret);

    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(LanPeer::PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;
    ret = bind(mSocketFd, (struct sockaddr*)&local_addr, sizeof(local_addr));
    assert(!ret);

    mBroadcastAddr = local_addr;
    mBroadcastAddr.sin_addr.s_addr = inet_addr("255.255.255.255");
}

LanPeer::~LanPeer() {
    if (mSocketFd > 0) close(mSocketFd);
}

std::optional<LanMessageUpdated> LanPeer::poll_updates() {
    std::lock_guard guard(mLock);
    if (mUpdatedSet.empty()) return {};
    LanMessageUpdated ret = *mUpdatedSet.begin();
    mUpdatedSet.erase(*mUpdatedSet.begin());
    return ret;
}

void LanPeer::enque_updateL(LanMessageUpdated message) {
    mUpdatedSet.insert(message);
}

void LanPeer::start_periodically_broadcast() {
    mIsBroadcasting.store(true);
    std::thread broadcasting_thread(&LanPeer::broadcast_as_host, this);
    broadcasting_thread.detach();
}

void LanPeer::stop_periodically_broadcast() { mIsBroadcasting.store(false); }

void LanPeer::start_listen_guest() {
    mIsListeningGuest.store(true);
    std::thread listen_guest_thread([this] {
        sf::TcpListener listener;
        if (listener.listen(PORT) != sf::Socket::Status::Done) {
            std::println("fail to listen on port {}", PORT);
            this->mIsListeningGuest.store(false);
        }

        sf::TcpSocket guest;
        if (listener.accept(guest) != sf::Socket::Status::Done)
            std::println("fail to accept guest connection");
        std::println("connected!");
    });
    listen_guest_thread.detach();
}

void LanPeer::stop_listen_guest() { mIsListeningGuest.store(false); }

void LanPeer::start_periodically_discover() {
    mIsDiscovering.store(true);
    std::thread discovering_thread(&LanPeer::receive_as_guest, this);
    discovering_thread.detach();
}

void LanPeer::stop_periodically_discover() { mIsDiscovering.store(false); }

void LanPeer::broadcast_as_host() {
    while (mIsBroadcasting.load()) {
        const char* msg =
            "hello there is a host";  // TODO: maybe send room name
        int len =
            sendto(mSocketFd, msg, strlen(msg), 0,
                   (struct sockaddr*)&mBroadcastAddr, sizeof(mBroadcastAddr));
        std::println("broadcasting with {} bytes", len);
        assert(len == strlen(msg));
        sleep(1);
    }
}

void LanPeer::receive_as_guest() {
    char buffer[64];
    struct sockaddr_in host_addr;
    socklen_t host_addr_len = sizeof(host_addr);
    while (mIsDiscovering.load()) {
        ssize_t len =
            recvfrom(mSocketFd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT,
                     (struct sockaddr*)&host_addr, &host_addr_len);

        //    assert(len > 0);
        buffer[len] = '\0';
        std::string sender_ip = inet_ntoa(host_addr.sin_addr);
        if (len != -1) {
            std::println("received {} bytes from {}:{}, msg: '{}'", len,
                         sender_ip, ntohs(host_addr.sin_port), buffer);
            mLock.lock();
            mLastHeard[sender_ip] = std::time(nullptr);
            mLock.unlock();
        } else {
            sleep(1);
            update_peer_info();
        }
    }
}

void LanPeer::update_peer_info() {
    std::lock_guard guard(mLock);
    std::time_t now = std::time(nullptr);
    bool updated = false;
    static constexpr int room_lost_timeout = 6 * BROADCAST_INTERVAL;

    // remove lost rooms
    for (auto it = mLastHeard.begin(); it != mLastHeard.end();) {
        if (std::difftime(now, it->second) > room_lost_timeout) {
            std::println("enque update for PeerInfo from {} to lost",
                         it->first);
            updated = true;
            mPeerInfoList.erase(it->first);
            it = mLastHeard.erase(it);
        } else
            it++;
    }

    // update signal strength for rooms that isn't lost
    for (const auto& lh : mLastHeard) {
        PeerInfo expected{.ip = lh.first};
        int time_elapsed = std::difftime(now, lh.second);
        if (time_elapsed <= 2 * LanPeer::BROADCAST_INTERVAL)
            expected.signal_strength = PeerInfo::SignalStrength::Strong;
        else if (time_elapsed <= 4 * LanPeer::BROADCAST_INTERVAL)
            expected.signal_strength = PeerInfo::SignalStrength::Medium;
        else if (time_elapsed <= room_lost_timeout)
            expected.signal_strength = PeerInfo::SignalStrength::Weak;

        if (mPeerInfoList[lh.first] != expected) {
            std::println("enque update for PeerInfo from {} to {}",
                         mPeerInfoList[lh.first].to_string(),
                         expected.to_string());
            mPeerInfoList[lh.first] = expected;
            updated = true;
        }
    }
    if (updated) enque_updateL(LanMessageUpdated::PeerInfoList);
}

std::vector<PeerInfo> LanPeer::get_peer_info_list() {
    std::lock_guard guard(mLock);
    std::vector<PeerInfo> ret;
    for (const auto& p : mPeerInfoList) ret.push_back(p.second);
    return ret;
}

bool LanPeer::connect_to_host(std::string host_ip) {
    // TODO: for now we just block on the connect call, which means main thread
    // is freezed
    sf::Socket::Status status =
        mTcpSocket.connect(*sf::IpAddress::resolve(host_ip), PORT);
    std::println("trying to connect to {}, got status {}", host_ip,
                 static_cast<int>(status));
    if (status != sf::Socket::Status::Done) return false;

    //    TODO();
	std::println("successfully connected to host!");
    return true;
}

void LanPeer::start_heartbeat_to_host() { TODO(); }

};  // namespace lan
