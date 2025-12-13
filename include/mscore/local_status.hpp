#pragma once

#include <string>

#include "../mine_cell.hpp"
#include "lan/lan.hpp"

enum class GameStatus {
    Lobby,
    Room,
    NotStarted,  // aka title screen
    Running,
};

class LocalStatus {
   public:
    LocalStatus();

    const GameStatus& game_status();
	void update();

    // network, in lobby
    void start_listen_room();
    void stop_listen_room();
    using RoomEntry = struct RoomEntry {
        std::string name = "";
        int signal_strength = 3;
        std::string to_string() const {
            return std::string("room ") + name + " " +
                   std::string(signal_strength, '|');
        }
    };
    using RoomEntryList = std::vector<RoomEntry>;
    const RoomEntryList& get_room_entry_list();

    // network, in room
    void host_room(/* TODO: game type, e.g. minesweeprt, geowartry...*/);
    void host_exit_room();   // host clicking exit room button calls this
    void guest_exit_room();  // guest clicking exit room button calls this
    using GuestInfo = struct GuestInfo {
        std::string nickname;
        std::string ip;
        std::string to_string() const { return nickname + " " + ip; }
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
