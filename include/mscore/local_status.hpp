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

    // network, in lobby
    using RoomEntry = struct {
        int id = -1;
        std::string name = "";
        std::string to_string() const {
            return "room " + std::to_string(id) + " " + name;
        }
    };
    using RoomEntryList = std::vector<RoomEntry>;
    const RoomEntryList& get_room_entry_list(bool& updated);
    const RoomEntryList& get_room_entry_list();

    // network, in room
    void host_game(/* TODO: game type, e.g. minesweeprt, geowartry...*/);
    using GuestInfo = struct {
        std::string nickname;
        std::string ip;
        std::string to_string() const { return nickname + " " + ip; }
    };
    using GuestInfoList = std::vector<GuestInfo>;
    const GuestInfoList& get_guest_info_list(bool& updated);
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
