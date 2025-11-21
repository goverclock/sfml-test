#include "../include/local_ui.hpp"

void LocalUI::init_field(const size_t row, const size_t col) {
    mFieldRects.resize(row, std::vector<sf::RectangleShape>(
                                col, sf::RectangleShape({100.f, 100.f})));
    for (size_t r = 0; r < row; r++)
        for (size_t c = 0; c < col; c++)
            mFieldRects[r][c].setPosition({110.f * c, 110.f * r});
}

void LocalUI::render_field(const MineField& mine_field) {
    assert(mine_field.size() == mFieldRects.size());
    assert(mine_field[0].size() == mFieldRects[0].size());
    for (size_t r = 0; r < mine_field.size(); r++)
        for (size_t c = 0; c < mine_field[0].size(); c++) {
            const MineCell& cell_status = mine_field[r][c];
            switch (cell_status.mCellStatus) {
                case MineCellStatus::Covered:
                    mFieldRects[r][c].setFillColor(sf::Color::Black);
                    mWindow.draw(mFieldRects[r][c]);
                    break;
                case MineCellStatus::Revealed:
                    mFieldRects[r][c].setFillColor(sf::Color::White);
                    mWindow.draw(mFieldRects[r][c]);
                    break;
                case MineCellStatus::Marked:
                    assert(false && "not implemented");
                    break;
            }
        }
}

std::optional<sf::Vector2i> LocalUI::get_rol_col_by_pos(
    const sf::Vector2i pos) {
    for (size_t r = 0; r < mFieldRects.size(); r++)
        for (size_t c = 0; c < mFieldRects[0].size(); c++) {
            if (mFieldRects[r][c].getGlobalBounds().contains(
                    {(float)pos.x, (float)pos.y})) {
                return {{(int)r, (int)c}};
            }
        }
    return {};
}
