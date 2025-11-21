#pragma once
#include <vector>

enum class MineCellStatus {
    Covered,
    Revealed,
    Marked,
};

struct MineCell {
    MineCellStatus mCellStatus;
    unsigned mMineNumber;
};

typedef std::vector<std::vector<MineCell>> MineField;
