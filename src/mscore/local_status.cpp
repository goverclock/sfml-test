#include <cassert>
#include <ctime>
#include <queue>

#include "ergonomics.hpp"
#include "mscore/local_status.hpp"

LocalStatus::LocalStatus() : mGameStatus(GameStatus::Lobby) {}

const GameStatus& LocalStatus::game_status() { return mGameStatus; }

void LocalStatus::update() {
    while (const std::optional lan_update = mLanPeer.poll_updates()) {
        switch (*lan_update) {
            case lan::LanMessageUpdated::RoomInfoList: {
                mRoomEntryList.clear();
                std::vector<lan::RoomInfo> room_info_list =
                    mLanPeer.get_room_info_list();
                for (const auto& pi : room_info_list) {
                    mRoomEntryList.push_back(RoomEntry{
                        .name = pi.ip + "'s room",
                        .ip = pi.ip,
                        .signal_strength =
                            static_cast<int>(pi.signal_strength) + 1});
                }
                break;
            }
            case lan::LanMessageUpdated::GuestInRoom: {
                mGuestInfoList.clear();
                const std::unordered_map<std::string, lan::GuestConnection>&
                    guest_conn_list = mLanPeer.get_connected_guest_info_list();
                for (const auto& gc : guest_conn_list) {
                    mGuestInfoList.push_back(GuestInfo{
                        .nickname = "guestnick",
                        .ip = gc.second.guest_ip(),
                        .signal_strength =
                            static_cast<int>(gc.second.signal_strength()) + 1});
                }
                break;
            }
            case lan::LanMessageUpdated::HostDismissRoom: {
                std::println("host dismissed room, returning to lobby");
                guest_exit_room();
                break;
            }
            default:
                UNREACHABLE();
                break;
        }
    }
}

const LocalStatus::RoomEntryList& LocalStatus::get_room_entry_list() {
    return mRoomEntryList;
}

void LocalStatus::host_exit_room() {
    mGameStatus = GameStatus::Lobby;
    mLanPeer.stop_periodically_broadcast();
    mLanPeer.stop_listen_guest();
    mLanPeer.disconnect_all_guests();
}

void LocalStatus::create_room() {
    mGameStatus = GameStatus::RoomAsHost;
    mLanPeer.start_periodically_broadcast();
    mLanPeer.start_listen_guest();
}

void LocalStatus::join_room(const RoomEntry& room_entry) {
    std::println("trying to join {}", room_entry.name);
    if (!mLanPeer.connect_to_host(room_entry.ip)) {
        std::println("fail to connect to {}", room_entry.ip);
        return;
    }
    mLanPeer.start_heartbeat_to_host();
    mGameStatus = GameStatus::RoomAsGuest;
}

void LocalStatus::guest_exit_room() {
    mLanPeer.stop_heartbeat_to_host();
    mLanPeer.disconnect_from_host();
    mGameStatus = GameStatus::Lobby;
}

void LocalStatus::start_discover_room() {
    mGameStatus = GameStatus::Lobby;
    mLanPeer.start_periodically_discover();
}

void LocalStatus::stop_discover_room() {
    mLanPeer.stop_periodically_discover();
}

const LocalStatus::GuestInfoList& LocalStatus::get_guest_info_list() {
    return mGuestInfoList;
}

void LocalStatus::start_game() {
    mGameStatus = GameStatus::Running;
    TODO();
}
