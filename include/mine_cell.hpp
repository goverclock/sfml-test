#pragma once
#include <vector>

enum class MineCellStatus {
    Covered,
    Revealed,
};

struct MineCell {
    MineCellStatus mCellStatus;
    unsigned mBombCount;
    bool mIsBomb;
};

using MineField = std::vector<std::vector<MineCell>>;
