#include "../include/local_status.hpp"
#include <cassert>

LocalStatus::LocalStatus()
    : mGameStatus(GameStatus::NotStarted),
      mMineField(std::make_unique<MineField>()) {}

void LocalStatus::start_game(size_t row, size_t col) {
    mRows = row;
    mCols = col;
    unsigned ind = 0;  // TEST:
    mMineField->clear();
    mMineField->resize(row);
    for (auto& r : (*mMineField))
        r.resize(col, MineCell{.mMineNumber = ++ind % 10});
}

const MineCell& LocalStatus::get_cell(size_t row, size_t col) {
    assert(row < mRows && col < mCols);
    return (*mMineField)[row][col];
}

size_t LocalStatus::field_rows() { return mRows; }
size_t LocalStatus::field_cols() { return mCols; }
