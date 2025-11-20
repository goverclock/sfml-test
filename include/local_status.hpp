#pragma once
#include <vector>
#include <memory>

enum class CellStatus {
    Covered,
    Revealed,
    Marked,
};

struct MineCell {
    CellStatus mCellStatus;
    unsigned mMineNumber;
};

typedef std::vector<std::vector<MineCell>> MineField;

enum class GameStatus {
    NotStarted,  // aka title screen
    Running,
    SummaryScreen,
};

class LocalStatus {
   public:
    LocalStatus();

    const GameStatus& game_status();
    void start_game(size_t row, size_t col);

    const MineCell& get_cell(size_t row, size_t col);
    size_t field_rows();
    size_t field_cols();
    void reveal(size_t row, size_t col);
    void mark(size_t row, size_t col);
    void unmark(size_t row, size_t col);

   private:
    size_t mRows;
    size_t mCols;
    GameStatus mGameStatus;
    std::unique_ptr<MineField> mMineField;
};
