#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../include/mine_cell.hpp"
#include "local_status.hpp"

struct MineCellUI {
    MineCellUI() : rect({cell_size, cell_size}), mine_number_text({}) {};
    constexpr static float cell_size = 90.f;
    sf::RectangleShape rect;
    std::optional<sf::Text> mine_number_text;
};

struct Renderable {
    virtual void render(sf::RenderWindow& w) = 0;
    virtual void handle_click_event(sf::RenderWindow& w, sf::Event e) = 0;
};

class GameRunningUI : public Renderable {
   public:
    GameRunningUI(LocalStatus& ls);
    std::optional<sf::Vector2i> get_rol_col_by_pos(const sf::Vector2i pos);
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

   private:
    LocalStatus& mLocalStatus;
    std::vector<std::vector<MineCellUI>> mFieldUI;
    sf::Font mFont;
};

class LocalUI : Renderable {
   public:
    LocalUI(LocalStatus& ls) : mLocalStatus(ls) {};
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

   private:
    LocalStatus& mLocalStatus;
    GameStatus mUIStatus;
    std::unique_ptr<Renderable> mStatusUI;
};
