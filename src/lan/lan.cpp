#include <cassert>
#include <cstring>
#include <print>
#include <thread>

#include "ergonomics.hpp"
#include "lan/lan.hpp"

namespace lan {

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
    std::thread broadcasting_thread([this] {
        sf::UdpSocket socket;
        while (mIsBroadcasting.load()) {
            static const char* str =
                "hello there is a host";  // maybe send room name
            std::array<char, 32> msg;
            std::strncpy(msg.data(), str, std::strlen(str));
            std::println("broadcasting with {} bytes", msg.size());
            if (socket.send(msg.data(), msg.size(), sf::IpAddress::Broadcast,
                            PORT) != sf::Socket::Status::Done) {
                std::println("fail to send broadcast message");
            }
            sleep(1);
        }
    });
    broadcasting_thread.detach();
}

void LanPeer::stop_periodically_broadcast() { mIsBroadcasting.store(false); }

void LanPeer::start_periodically_discover() {
    mIsDiscovering.store(true);
    std::thread discovering_thread([this] {
        sf::UdpSocket socket;
        socket.setBlocking(false);
        if (socket.bind(PORT) != sf::Socket::Status::Done) {
            std::println("fail to UDP bind port {}", PORT);
            mIsDiscovering.store(false);
        }

        std::optional<sf::IpAddress> sender_ip = sf::IpAddress::Broadcast;
        unsigned short sender_port;
        std::array<char, 32> data;
        size_t len = 0;
        while (mIsDiscovering.load()) {
            const sf::Socket::Status status = socket.receive(
                data.data(), data.size(), len, sender_ip, sender_port);
            switch (status) {
                case sf::Socket::Status::Done:
                    std::println("received {} bytes from {}:{}, data: '{}' ",
                                 len, sender_ip->toString(), sender_port,
                                 data.data());
                    mLock.lock();
                    mLastHeard[sender_ip->toString()] = std::time(nullptr);
                    mLock.unlock();
                    break;
                case sf::Socket::Status::NotReady:
                    update_peer_info();
                    sleep(1);
                    break;
                default:
                    std::println("discovery status: {}",
                                 static_cast<int>(status));
            }
        }
    });
    discovering_thread.detach();
}

void LanPeer::stop_periodically_discover() { mIsDiscovering.store(false); }

void LanPeer::start_listen_guest() {
    mIsListeningGuest.store(true);
    std::thread listen_guest_thread([this] {
        sf::TcpListener listener;
        if (listener.listen(PORT) != sf::Socket::Status::Done) {
            std::println("fail to TCP listen on port {}", PORT);
            this->mIsListeningGuest.store(false);
        }

        sf::TcpSocket guest;
        while (mIsListeningGuest.load()) {
            if (listener.accept(guest) != sf::Socket::Status::Done)
                std::println("fail to accept guest connection");
            std::println("connected!");
        }
    });
    listen_guest_thread.detach();
}

void LanPeer::stop_listen_guest() { mIsListeningGuest.store(false); }

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
        mToHostTcpSocket.connect(*sf::IpAddress::resolve(host_ip), PORT);
    std::println("trying to connect to {}:{}, got status {}", host_ip, PORT,
                 static_cast<int>(status));
    if (status != sf::Socket::Status::Done) return false;

    //    TODO();
    std::println("successfully connected to host!");
    return true;
}

void LanPeer::start_heartbeat_to_host() { TODO(); }

};  // namespace lan
