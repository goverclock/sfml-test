#pragma once
#include "../mine_cell.hpp"
#include <memory>

enum class GameStatus {
    NotStarted,  // aka title screen
    Running,
};

class LocalStatus {
   public:
    LocalStatus();

    void start_game(size_t row, size_t col);

    const GameStatus& game_status();
    const MineCell& get_cell(size_t row, size_t col);
    const MineField& get_field();
    size_t field_rows();
    size_t field_cols();
    void reveal_cell(size_t row, size_t col);

   private:
    size_t mRows;
    size_t mCols;
    GameStatus mGameStatus;
    MineField mMineField;
};
