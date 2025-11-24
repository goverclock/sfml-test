#include <iostream>

#include "../include/local_ui.hpp"

GameRunningUI::GameRunningUI(LocalStatus& ls)
    : mLocalStatus(ls), mFont("resource/JetBrainsMono-Regular.ttf") {
    size_t row = mLocalStatus.field_rows();
    size_t col = mLocalStatus.field_cols();
    mFieldUI.resize(row, std::vector<MineCellUI>(col, MineCellUI()));
    for (size_t r = 0; r < row; r++)
        for (size_t c = 0; c < col; c++) {
            mFieldUI[r][c].rect.setPosition(
                {10.f + (MineCellUI::cell_size + 10.f) * c,
                 10.f + (MineCellUI::cell_size + 10.f) * r});
        }
}

std::optional<sf::Vector2i> GameRunningUI::get_rol_col_by_pos(
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

void GameRunningUI::render(sf::RenderWindow& window) {
    const auto mine_field = mLocalStatus.get_field();
    assert(mine_field.size() == mFieldUI.size());
    assert(mine_field[0].size() == mFieldUI[0].size());
    for (size_t r = 0; r < mine_field.size(); r++)
        for (size_t c = 0; c < mine_field[0].size(); c++) {
            const MineCell& cell_status = mine_field[r][c];
            MineCellUI& cell_ui = mFieldUI[r][c];
            switch (cell_status.mCellStatus) {
                case MineCellStatus::Covered:
                    cell_ui.rect.setFillColor(sf::Color::Black);
                    window.draw(cell_ui.rect);
                    break;
                case MineCellStatus::Revealed:
                    cell_ui.rect.setFillColor(cell_status.mIsBomb
                                                  ? sf::Color::Red
                                                  : sf::Color::White);
                    window.draw(cell_ui.rect);
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
                        window.draw(*cell_ui.mine_number_text);
                    break;
                case MineCellStatus::Marked:
                    cell_ui.rect.setFillColor(sf::Color::Yellow);
                    window.draw(cell_ui.rect);
                    break;
            }
        }
}

void GameRunningUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    const sf::Event::MouseButtonPressed* mouse_button_pressed =
        e.getIf<sf::Event::MouseButtonPressed>();
    assert(mouse_button_pressed);
    const auto mouse_pos = sf::Mouse::getPosition(w);
    std::cout << "mouse at: " << mouse_pos.x << " " << mouse_pos.y << std::endl;
    const auto rol_col = get_rol_col_by_pos(mouse_pos);
    if (!rol_col) {
        std::cout << "not clicking on any cell" << std::endl;
        return;
    }
    std::cout << "clicking row col: " << rol_col->x << " " << rol_col->y
              << std::endl;
    if (mouse_button_pressed->button == sf::Mouse::Button::Left)
        mLocalStatus.reveal_cell(rol_col->x, rol_col->y);
    if (mouse_button_pressed->button == sf::Mouse::Button::Right)
        mLocalStatus.mark_cell(rol_col->x, rol_col->y);
}

void LocalUI::render(sf::RenderWindow& w) {
    if (!mStatusUI || mUIStatus != mLocalStatus.game_status()) {
        switch (mLocalStatus.game_status()) {
            case GameStatus::Running:
                mStatusUI.reset(new GameRunningUI(mLocalStatus));
                break;
            default:
                assert(false && "not implemented");
                break;
        }
        mUIStatus = mLocalStatus.game_status();
    }

    switch (mUIStatus) {
        case GameStatus::Running:
            mStatusUI->render(w);
            break;
        default:
            assert(false && "not implemented");
            break;
    }
}

void LocalUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    assert(mStatusUI);
    mStatusUI->handle_click_event(w, e);
}
