#include <cassert>
#include <ctime>
#include <queue>

#include "ergonomics.hpp"
#include "mscore/local_status.hpp"

LocalStatus::LocalStatus() : mGameStatus(GameStatus::Lobby), mMineField() {}

const GameStatus& LocalStatus::game_status() { return mGameStatus; }

void LocalStatus::update() {
    while (const std::optional lan_update = mLanPeer.poll_updates()) {
        switch (*lan_update) {
            case lan::LanMessageUpdated::PeerInfoList: {
                mRoomEntryList.clear();
                std::vector<lan::PeerInfo> peer_info_list =
                    mLanPeer.get_peer_info_list();
                for (const auto& pi : peer_info_list) {
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
                std::vector<lan::ConnectedGuestInfo> connected_guest_list =
                    mLanPeer.get_connected_guest_info_list();
                for (const auto& cg : connected_guest_list) {
                    mGuestInfoList.push_back(GuestInfo{
                        .nickname = "guestnick",
                        .ip = cg.guest_ip,
                    });
                }
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
    TODO();
    // mLanPeer.disconnect_all_guests();
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

void LocalStatus::start_game(size_t row, size_t col) {
    mGameStatus = GameStatus::Running;

    // init field
    mRows = row;
    mCols = col;
    mMineField.clear();
    mMineField.resize(row);
    for (size_t r = 0; r < row; r++) {
        mMineField[r].resize(col);
        for (size_t c = 0; c < col; c++)
            mMineField[r][c] = MineCell{
                .mCellStatus = MineCellStatus::Covered,
                .mBombCount = 0,
                .mIsBomb = false,
            };
    }

    std::srand(std::time({}));
    std::vector<std::pair<size_t, size_t>> bombs;
    // TODO: duplicate is possible
    while (bombs.size() < 10)
        bombs.push_back(
            {(size_t)(std::rand() % row), (size_t)(std::rand() % col)});
    for (const auto& b : bombs) mMineField[b.first][b.second].mIsBomb = true;

    for (size_t r = 0; r < row; r++) {
        for (size_t c = 0; c < col; c++) {
            auto& cell = mMineField[r][c];
            if (cell.mIsBomb) continue;
            std::pair<size_t, size_t> check_coords[9] = {
                {r - 1, c - 1}, {r - 1, c}, {r - 1, c + 1},
                {r, c - 1},     {r, c},     {r, c + 1},
                {r + 1, c - 1}, {r + 1, c}, {r + 1, c + 1},
            };

            for (size_t i = 0; i < 9; i++) {
                size_t nr = check_coords[i].first, nc = check_coords[i].second;
                if (nr < 0 || nr >= row || nc < 0 || nc >= col) continue;
                if (mMineField[nr][nc].mIsBomb) cell.mBombCount++;
            }
        }
    }
}

const MineCell& LocalStatus::get_cell(size_t row, size_t col) {
    assert(row < mRows && col < mCols);
    return mMineField[row][col];
}

const MineField& LocalStatus::get_field() { return mMineField; }

size_t LocalStatus::field_rows() { return mRows; }

size_t LocalStatus::field_cols() { return mCols; }

void LocalStatus::reveal_cell(size_t row, size_t col) {
    assert(row < mRows && col < mCols);
    if (mMineField[row][col].mCellStatus == MineCellStatus::Revealed) return;
    if (mMineField[row][col].mIsBomb) {
        mMineField[row][col].mCellStatus = MineCellStatus::Revealed;
        // TODO: mark as failed
        return;
    }

    const int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    std::queue<std::pair<int, int>> q;
    q.push({(int)row, (int)col});
    while (!q.empty()) {
        const auto cur = q.front();
        q.pop();

        auto& cur_cell = mMineField[cur.first][cur.second];
        cur_cell.mCellStatus = MineCellStatus::Revealed;
        if (!cur_cell.mBombCount)
            for (int i = 0; i < 8; i++) {
                int nr = cur.first + dr[i];
                int nc = cur.second + dc[i];
                if (nr < 0 || nr >= mRows || nc < 0 || nc >= mCols) continue;
                if (mMineField[nr][nc].mCellStatus != MineCellStatus::Revealed)
                    q.push({nr, nc});
            }
    }
}
