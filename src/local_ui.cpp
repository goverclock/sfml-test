#include "../include/local_ui.hpp"

void LocalUI::init_field(const size_t row, const size_t col) {
    mFieldUI.resize(row, std::vector<MineCellUI>(col, MineCellUI()));
    for (size_t r = 0; r < row; r++)
        for (size_t c = 0; c < col; c++) {
            mFieldUI[r][c].rect.setPosition(
                {10.f + (MineCellUI::cell_size + 10.f) * c,
                 10.f + (MineCellUI::cell_size + 10.f) * r});
        }
}

void LocalUI::render_field(const MineField& mine_field) {
    assert(mine_field.size() == mFieldUI.size());
    assert(mine_field[0].size() == mFieldUI[0].size());
    for (size_t r = 0; r < mine_field.size(); r++)
        for (size_t c = 0; c < mine_field[0].size(); c++) {
            const MineCell& cell_status = mine_field[r][c];
            MineCellUI& cell_ui = mFieldUI[r][c];
            switch (cell_status.mCellStatus) {
                case MineCellStatus::Covered:
                    cell_ui.rect.setFillColor(sf::Color::Black);
                    mWindow.draw(cell_ui.rect);
                    break;
                case MineCellStatus::Revealed:
                    cell_ui.rect.setFillColor(cell_status.mIsBomb
                                                  ? sf::Color::Red
                                                  : sf::Color::White);
                    mWindow.draw(cell_ui.rect);
                    if (cell_status.mBombCount && !cell_ui.mine_number_text) {
                        cell_ui.mine_number_text = sf::Text(
                            mFont, std::to_string(cell_status.mBombCount));
                        cell_ui.mine_number_text->setCharacterSize(50);
                        cell_ui.mine_number_text->setFillColor(sf::Color::Red);
                        cell_ui.mine_number_text->setPosition(
                            {10.f + (MineCellUI::cell_size + 10.f) * c,
                             10.f + (MineCellUI::cell_size + 10.f) * r});
                    }
                    if (cell_ui.mine_number_text)
                        mWindow.draw(*cell_ui.mine_number_text);
                    break;
                case MineCellStatus::Marked:
                    cell_ui.rect.setFillColor(sf::Color::Yellow);
                    mWindow.draw(cell_ui.rect);
                    break;
            }
        }
}

std::optional<sf::Vector2i> LocalUI::get_rol_col_by_pos(
    const sf::Vector2i pos) {
    for (size_t r = 0; r < mFieldUI.size(); r++)
        for (size_t c = 0; c < mFieldUI[0].size(); c++) {
            if (mFieldUI[r][c].rect.getGlobalBounds().contains(
                    {(float)pos.x, (float)pos.y})) {
                return {{(int)r, (int)c}};
            }
        }
    return {};
}
