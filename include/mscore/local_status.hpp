#pragma once

#include <string>

#include "../mine_cell.hpp"
#include "lan/lan.hpp"

enum class GameStatus {
    Lobby,
    RoomAsHost,
    RoomAsGuest,
    NotStarted,  // aka title screen
    Running,
};

enum class GameType {
    MineSweeper,
    Geowartry,
};

class LocalStatus {
   public:
    LocalStatus();

    const GameStatus& game_status();
    void update();

    using RoomEntry = struct RoomEntry {
        std::string name = "";
        std::string ip;
        int signal_strength;
        int player_count;
        GameType game_type;
        std::string to_string() const {
            return name + " " + std::string(signal_strength, '|');
        }
    };
    using RoomEntryList = std::vector<RoomEntry>;

    // network, in lobby
    const RoomEntryList& get_room_entry_list();
    void create_room(/* TODO: game type, e.g. minesweeprt, geowartry...*/);
    void join_room(const RoomEntry& room_entry);
    void start_discover_room();
    void stop_discover_room();

    // network, in room
    void host_exit_room();   // host clicking exit room button calls this
    void guest_exit_room();  // guest clicking exit room button calls this
    using GuestInfo = struct GuestInfo {
        std::string nickname;
        std::string ip;
        int signal_strength;
        std::string to_string() const {
            return nickname + " " + ip + " " +
                   std::string(signal_strength, '|');
        }
    };
    using GuestInfoList = std::vector<GuestInfo>;
    const GuestInfoList& get_guest_info_list();

    // minesweeper
    void start_game(size_t row, size_t col);
    const MineCell& get_cell(size_t row, size_t col);
    const MineField& get_field();
    size_t field_rows();
    size_t field_cols();
    void reveal_cell(size_t row, size_t col);

   private:
    // network
    lan::LanPeer mLanPeer;
    RoomEntryList mRoomEntryList;
    GuestInfoList mGuestInfoList;
    // minesweeper
    GameStatus mGameStatus;
    size_t mRows;
    size_t mCols;
    MineField mMineField;
};
