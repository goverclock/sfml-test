#include "../include/local_status.hpp"
#include <cassert>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <iostream>

LocalStatus::LocalStatus()
    : mGameStatus(GameStatus::NotStarted), mMineField() {}

void LocalStatus::start_game(size_t row, size_t col) {
    mRows = row;
    mCols = col;
    mMineField.clear();
    mMineField.resize(row);
    // for (size_t r = 0; r < row; r++) {
    //     mMineField[r].resize(col);
    //     for (size_t c = 0; c < col; c++)  // TEST:
    //         mMineField[r][c] = MineCell{
    //             .mCellStatus = ((r + c) % 2) ? MineCellStatus::Covered
    //                                          : MineCellStatus::Revealed,
    //             .mBombCount = static_cast<unsigned>(r + c) % 10};
    // }
    for (size_t r = 0; r < row; r++) {
        mMineField[r].resize(col);
        for (size_t c = 0; c < col; c++)
            mMineField[r][c] = MineCell{
                .mCellStatus = MineCellStatus::Covered,
                .mIsBomb = false,
                .mBombCount = 0,
            };
    }

    std::srand(std::time({}));
    std::vector<std::pair<size_t, size_t>> bombs;
    // TEST: duplicate is possible
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

void LocalStatus::reveal(size_t row, size_t col) {
    assert(row < mRows && col < mCols);
    if (mMineField[row][col].mCellStatus == MineCellStatus::Revealed) return;
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

void LocalStatus::mark(size_t row, size_t col) {
    assert(row < mRows && col < mCols);
    auto& cell = mMineField[row][col];
    if (cell.mCellStatus != MineCellStatus::Covered) return;
    cell.mCellStatus = MineCellStatus::Marked;
}