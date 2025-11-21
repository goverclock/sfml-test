#include "../include/local_status.hpp"
#include <cassert>

LocalStatus::LocalStatus()
    : mGameStatus(GameStatus::NotStarted), mMineField() {}

void LocalStatus::start_game(size_t row, size_t col) {
    mRows = row;
    mCols = col;
    mMineField.clear();
    mMineField.resize(row);
    for (size_t r = 0; r < row; r++) {
        mMineField[r].resize(col);
        for (size_t c = 0; c < col; c++)  // TEST:
            mMineField[r][c] = MineCell{
                .mCellStatus = ((r + c) % 2) ? MineCellStatus::Covered
                                             : MineCellStatus::Revealed,
                .mMineNumber = static_cast<unsigned>(r + c) % 10};
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
    mMineField[row][col].mCellStatus = MineCellStatus::Revealed;
}