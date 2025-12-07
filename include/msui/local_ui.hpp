#pragma once
#include "mine_cell.hpp"
#include "mscore/local_status.hpp"
#include "renderable.hpp"
#include "widget/list_view.hpp"

struct MineCellUI {
    MineCellUI() : rect({cell_size, cell_size}), mine_number_text({}) {};
    constexpr static float cell_size = 90.f;
    sf::RectangleShape rect;
    std::optional<sf::Text> mine_number_text;
    bool is_marked = false;
};

class MineFieldUI : public Renderable {
   public:
    MineFieldUI(LocalStatus& ls);
    virtual ~MineFieldUI();
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

   private:
    std::optional<sf::Vector2i> get_rol_col_by_pos(const sf::Vector2i pos);
    void mark_cell(size_t row, size_t col);
    void unmark_cell(size_t row, size_t col);
    LocalStatus& mLocalStatus;
    std::vector<std::vector<MineCellUI>> mFieldUI;
    sf::Font mFont;
};

struct RoomEntry {
    int number = 123;
    std::string to_string() const { return "room " + std::to_string(number); }
};

class LobbyUI : public Renderable {
   public:
    LobbyUI();
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

   private:
    sf::Font mFont;
    // TODO: create a button class, which is also Renderable
    sf::RectangleShape mCreateRoomBtn;
    sf::Text mTextCreateRoom;
    widget::ListView<RoomEntry> mListView;
};

class GameTitleUI : public Renderable {
   public:
    GameTitleUI(LocalStatus& ls);
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

   private:
    LocalStatus& mLocalStatus;
    // TODO: refactor into a ResourceManager to store mFont for all class
    sf::Font mFont;
    // TODO: create a button class, which is also Renderable
    sf::RectangleShape mStartButton10_10;
    sf::Text mText10_10;
    sf::RectangleShape mStartButton10_15;
    sf::Text mText10_15;
};

class GameRunningUI : public Renderable {
   public:
    GameRunningUI(LocalStatus& ls);
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

   private:
    LocalStatus& mLocalStatus;
    MineFieldUI mMineFieldUI;
};

class LocalUI : Renderable {
   public:
    LocalUI(LocalStatus& ls) : mLocalStatus(ls) {};
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

   private:
    LocalStatus& mLocalStatus;
    GameStatus mUIStatus;
    std::unique_ptr<Renderable> mStatusUISP;
};
