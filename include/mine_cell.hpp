#pragma once
#include <vector>

enum class MineCellStatus {
    Covered,
    Revealed,
    Marked,
};

struct MineCell {
    MineCellStatus mCellStatus;
    bool mIsBomb;
    unsigned mBombCount;
};

using MineField = std::vector<std::vector<MineCell>>;
