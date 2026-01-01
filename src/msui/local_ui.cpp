#include <print>

#include "ergonomics.hpp"
#include "msui/local_ui.hpp"

RoomAsHostUI::RoomAsHostUI(LocalStatus& ls)
    : mLocalStatus(ls), mPlayerListView(ls.get_guest_info_list()) {
    mExitRoomBtn.set_text("Exit room");
    mExitRoomBtn.on_click([&] {
        std::println("RoomAsHost: clicked on Exit room");
        mLocalStatus.host_exit_room();
    });

    mStartGameBtn.set_text("Start game");
    mStartGameBtn.on_click([&] { ls.start_game(10, 10); });  // TEST:
}

void RoomAsHostUI::render(sf::RenderWindow& w) {
    sf::Vector2u window_size = w.getSize();
    float x = window_size.x - mExitRoomBtn.get_size().x;
    float y = window_size.y - 200.f;

    mExitRoomBtn.setPosition({x, y});
    w.draw(mExitRoomBtn);

    mStartGameBtn.setPosition({0.f, y});
    w.draw(mStartGameBtn);

    mPlayerListView.setPosition({100.f, 100.f});
    w.draw(mPlayerListView);
}

void RoomAsHostUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    mExitRoomBtn.handle_click_event(w, e);
    mStartGameBtn.handle_click_event(w, e);
}

RoomAsGuestUI::RoomAsGuestUI(LocalStatus& ls)
    : mLocalStatus(ls), mPlayerListView(ls.get_guest_info_list()) {
    mExitRoomBtn.set_text("Exit room");
    mExitRoomBtn.on_click([&] {
        std::println("RoomAsGuest: clicked on Exit room");
        mLocalStatus.guest_exit_room();
    });
}

void RoomAsGuestUI::render(sf::RenderWindow& w) {
    sf::Vector2u window_size = w.getSize();
    float x = window_size.x - mExitRoomBtn.get_size().x;
    float y = window_size.y - 200.f;

    mExitRoomBtn.setPosition({x, y});
    w.draw(mExitRoomBtn);

    mPlayerListView.setPosition({100.f, 100.f});
    w.draw(mPlayerListView);
}

void RoomAsGuestUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    mExitRoomBtn.handle_click_event(w, e);
}

LobbyUI::LobbyUI(LocalStatus& ls)
    : mLocalStatus(ls), mRoomListView(ls.get_room_entry_list()) {
    mCreateRoomBtn.set_text("Create room");
    mCreateRoomBtn.on_click([&] {
        std::println("Lobby: clicked on Create room");
        mLocalStatus.create_room();
    });

    mRoomListView.on_item_click([&](const LocalStatus::RoomEntry& room_entry) {
        ls.join_room(room_entry);
    });

    ls.start_discover_room();
}

LobbyUI::~LobbyUI() { mLocalStatus.stop_discover_room(); }

void LobbyUI::render(sf::RenderWindow& w) {
    sf::Vector2u window_size = w.getSize();
    float x = 0;
    float y = window_size.y - 200.f;

    mCreateRoomBtn.setPosition({x, y});
    w.draw(mCreateRoomBtn);

    mRoomListView.setPosition({100.f, 100.f});
    mRoomListView.render(w);
}

void LobbyUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    mCreateRoomBtn.handle_click_event(w, e);
    mRoomListView.handle_click_event(w, e);
}

GameTitleUI::GameTitleUI(LocalStatus& ls) : mLocalStatus(ls) {
    mStart10x10Btn.set_size({300.f, 140.f});
    mStart10x10Btn.set_text("Start\n10x10");
    mStart10x10Btn.on_click([&] {
        std::println("Title: clicked on Start 10x10");
        mLocalStatus.start_game(10, 10);
    });

    mStart10x15Btn.set_size({300.f, 140.f});
    mStart10x15Btn.set_text("Start\n10x15");
    mStart10x15Btn.on_click([&] {
        std::println("Title: clicked on Start 10x15");
        mLocalStatus.start_game(10, 15);
    });
}

void GameTitleUI::render(sf::RenderWindow& w) {
    sf::Vector2u window_size = w.getSize();
    float x = window_size.x / 2 - mStart10x10Btn.get_size().x / 2;
    float y = window_size.y / 2;
    mStart10x10Btn.setPosition({x, y});
    mStart10x15Btn.setPosition({x, y + 200.f});
    w.draw(mStart10x10Btn);
    w.draw(mStart10x15Btn);
}

void GameTitleUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    mStart10x10Btn.handle_click_event(w, e);
    mStart10x15Btn.handle_click_event(w, e);
}

MineFieldUI::MineFieldUI(LocalStatus& ls)
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

// FUCK C++ compiler
MineFieldUI::~MineFieldUI() {};

void MineFieldUI::render(sf::RenderWindow& window) {
    const auto mine_field = mLocalStatus.get_field();
    assert(mine_field.size() == mFieldUI.size());
    assert(mine_field[0].size() == mFieldUI[0].size());
    for (size_t r = 0; r < mine_field.size(); r++)
        for (size_t c = 0; c < mine_field[0].size(); c++) {
            const MineCell& cell_status = mine_field[r][c];
            MineCellUI& cell_ui = mFieldUI[r][c];
            switch (cell_status.mCellStatus) {
                case MineCellStatus::Covered:
                    if (cell_ui.is_marked)
                        cell_ui.rect.setFillColor(sf::Color::Yellow);
                    else
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
                default:
                    assert(false && "unknown cell status");
                    break;
            }
        }
}

void MineFieldUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    const sf::Event::MouseButtonPressed* mouse_button_pressed =
        e.getIf<sf::Event::MouseButtonPressed>();
    assert(mouse_button_pressed);
    const auto mouse_pos = sf::Mouse::getPosition(w);
    std::println("mouse at: ({}, {})", mouse_pos.x, mouse_pos.y);
    const auto rol_col = get_rol_col_by_pos(mouse_pos);
    if (!rol_col) {
        std::println("not clicking on any cell");
        return;
    }
    std::println("clicking row col: ({}, {})", rol_col->x, rol_col->y);
    if (mouse_button_pressed->button == sf::Mouse::Button::Left)
        mLocalStatus.reveal_cell(rol_col->x, rol_col->y);
    if (mouse_button_pressed->button == sf::Mouse::Button::Right) {
        if (!mFieldUI[rol_col->x][rol_col->y].is_marked)
            mark_cell(rol_col->x, rol_col->y);
        else
            unmark_cell(rol_col->x, rol_col->y);
    }
}

std::optional<sf::Vector2i> MineFieldUI::get_rol_col_by_pos(
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

void MineFieldUI::mark_cell(size_t row, size_t col) {
    assert(row < mFieldUI.size() && col < mFieldUI[0].size());
    auto& cell = mLocalStatus.get_cell(row, col);
    if (cell.mCellStatus != MineCellStatus::Covered) return;
    mFieldUI[row][col].is_marked = true;
}

void MineFieldUI::unmark_cell(size_t row, size_t col) {
    assert(row < mFieldUI.size() && col < mFieldUI[0].size());
    auto& cell = mLocalStatus.get_cell(row, col);
    if (cell.mCellStatus != MineCellStatus::Covered) return;
    mFieldUI[row][col].is_marked = false;
}

GameRunningUI::GameRunningUI(LocalStatus& ls)
    : mLocalStatus(ls), mMineFieldUI(ls) {}

void GameRunningUI::render(sf::RenderWindow& window) {
    mMineFieldUI.render(window);
}

void GameRunningUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    mMineFieldUI.handle_click_event(w, e);
}

void LocalUI::render(sf::RenderWindow& w) {
    std::unique_ptr<MineFieldUI> x;
    if (!mStatusUISP || mUIStatus != mLocalStatus.game_status()) {
        switch (mLocalStatus.game_status()) {
            case GameStatus::Lobby:
                std::println("to Lobby");
                mStatusUISP.reset(new LobbyUI(mLocalStatus));
                break;
            case GameStatus::RoomAsHost:
                std::println("to RoomAsHost");
                mStatusUISP.reset(new RoomAsHostUI(mLocalStatus));
                break;
            case GameStatus::RoomAsGuest:
                std::println("to RoomAsGuest");
                mStatusUISP.reset(new RoomAsGuestUI(mLocalStatus));
                break;
            case GameStatus::NotStarted:
                std::println("to GameTitle");
                mStatusUISP.reset(new GameTitleUI(mLocalStatus));
                break;
            case GameStatus::Running:
                std::println("to GameRunning");
                mStatusUISP.reset(new GameRunningUI(mLocalStatus));
                break;
            default:
                UNREACHABLE();
                break;
        }
        mUIStatus = mLocalStatus.game_status();
    }

    mStatusUISP->render(w);
}

void LocalUI::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    assert(mStatusUISP);
    mStatusUISP->handle_click_event(w, e);
}
