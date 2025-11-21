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

typedef std::vector<std::vector<MineCell>> MineField;
