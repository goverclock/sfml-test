#include <cassert>
#include <cstring>
#include <print>
#include <thread>

#include "ergonomics.hpp"
#include "lan/lan.hpp"
#include "lan/packet.hpp"

namespace lan {

std::optional<LanMessageUpdated> LanPeer::poll_updates() {
    std::lock_guard guard(mLock);
    if (mUpdatedSet.empty()) return {};
    LanMessageUpdated ret = *mUpdatedSet.begin();
    mUpdatedSet.erase(*mUpdatedSet.begin());
    return ret;
}

void LanPeer::enque_updateL(LanMessageUpdated message) {
    assert(!mLock.try_lock());
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
            sf::sleep(sf::seconds(1.f));
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
            std::println("fail to UDP bind port {}",
                         PORT);  // TODO: try to bind again later
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
                    mRoomLastHeard[sender_ip->toString()] = std::time(nullptr);
                    mLock.unlock();
                    break;
                case sf::Socket::Status::NotReady:
                    update_room_info();
                    sf::sleep(sf::seconds(1.f));
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
        sf::SocketSelector selector;
        selector.add(listener);

        while (mIsListeningGuest.load()) {
            // new guest connection ?
            sf::TcpSocket guest;
            if (selector.wait(sf::seconds(1.0f))) {
                // no need to call isReady since we added only the listener
                const sf::Socket::Status status = listener.accept(guest);
                switch (status) {
                    case sf::Socket::Status::Done: {
                        std::println("new guest connected!: {} ",
                                     guest.getRemoteAddress()->toString());
                        mGuestConnectionManager.add_connection(
                            std::move(guest));
                        break;
                    }
                    case sf::Socket::Status::NotReady:
                        break;
                    default:
                        std::println("listen guest failed with status: {} ",
                                     static_cast<int>(status));
                }
            } else
                // only update guest info when no new guest connection is
                // coming, i.e. when selector.wait() returns false
                update_connected_guest_info();
        }
    });
    listen_guest_thread.detach();
}

void LanPeer::stop_listen_guest() { mIsListeningGuest.store(false); }

void LanPeer::disconnect_all_guests() {
    mGuestConnectionManager.disconnect_all();
}

void LanPeer::update_connected_guest_info() {
    std::lock_guard guard(mLock);
    if (mGuestConnectionManager.check_guest_info_update())
        enque_updateL(LanMessageUpdated::GuestInRoom);
}

const std::unordered_map<std::string, GuestConnection>&
LanPeer::get_connected_guest_info_list() {
    return mGuestConnectionManager.guest_connection_list();
}

void LanPeer::update_room_info() {
    std::lock_guard guard(mLock);
    std::time_t now = std::time(nullptr);
    bool updated = false;
    static constexpr int room_lost_timeout = 6 * BROADCAST_INTERVAL;

    // remove lost rooms
    for (auto it = mRoomLastHeard.begin(); it != mRoomLastHeard.end();) {
        if (std::difftime(now, it->second) > room_lost_timeout) {
            std::println("enque update for RoomInfo from {} to lost",
                         it->first);
            updated = true;
            mRoomInfoList.erase(it->first);
            it = mRoomLastHeard.erase(it);
        } else
            it++;
    }

    // update signal strength for rooms that isn't lost
    for (const auto& lh : mRoomLastHeard) {
        RoomInfo expected{.ip = lh.first};
        int time_elapsed = std::difftime(now, lh.second);
        if (time_elapsed <= 2 * LanPeer::BROADCAST_INTERVAL)
            expected.signal_strength = SignalStrength::Strong;
        else if (time_elapsed <= 4 * LanPeer::BROADCAST_INTERVAL)
            expected.signal_strength = SignalStrength::Medium;
        else if (time_elapsed <= room_lost_timeout)
            expected.signal_strength = SignalStrength::Weak;

        if (mRoomInfoList[lh.first] != expected) {
            std::println("enque update for RoomInfo from {} to {}",
                         mRoomInfoList[lh.first].to_string(),
                         expected.to_string());
            mRoomInfoList[lh.first] = expected;
            updated = true;
        }
    }
    if (updated) enque_updateL(LanMessageUpdated::RoomInfoList);
}

std::vector<RoomInfo> LanPeer::get_room_info_list() {
    std::lock_guard guard(mLock);
    std::vector<RoomInfo> ret;
    for (const auto& p : mRoomInfoList) ret.push_back(p.second);
    return ret;
}

bool LanPeer::connect_to_host(std::string host_ip) {
    // TODO: for now we just block on the connect call, which means main
    // thread is freezed
    sf::Socket::Status status =
        mToHostTcpSocket.connect(*sf::IpAddress::resolve(host_ip), PORT);
    std::println("trying to connect to {}:{}, got status {}", host_ip, PORT,
                 static_cast<int>(status));
    if (status == sf::Socket::Status::Done) {
        std::println("successfully connected to host!");
        return true;
    }
    std::println("fail to connected to host!");
    return false;
}

void LanPeer::disconnect_from_host() {
    std::println("disconnecting from host");
    mToHostTcpSocket.disconnect();
}

void LanPeer::start_heartbeat_to_host() {
    mIsHeartbeating.store(true);
    std::thread heartbeat_thread([this] {
        sf::Packet heartbeat_packet;
        lan::packet::Heartbeat heartbeat{*sf::IpAddress::getLocalAddress(),
                                         *mToHostTcpSocket.getRemoteAddress()};
        heartbeat_packet << heartbeat;
        while (mIsHeartbeating.load()) {
            std::println("sending heartbeat packet to host: from {} to {}",
                         heartbeat.from.toString(), heartbeat.to.toString());
            assert(mToHostTcpSocket.isBlocking());
            sf::Socket::Status status = mToHostTcpSocket.send(heartbeat_packet);
            assert(
                status ==
                sf::Socket::Status::Done);  // TODO: host is lost,
                                            // guest exit room, use enque update
            sf::sleep(sf::seconds(1.f));
        }
    });
    heartbeat_thread.detach();
}

void LanPeer::stop_heartbeat_to_host() { mIsHeartbeating.store(false); }
};  // namespace lan
