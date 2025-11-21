#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../include/mine_cell.hpp"

class LocalUI {
   public:
    LocalUI(sf::RenderWindow& w) : mWindow(w) {};
    void init_field(const size_t row, const size_t col);
    void render_field(const MineField& mine_field);
    std::optional<sf::Vector2i> get_rol_col_by_pos(const sf::Vector2i pos);

   private:
    sf::RenderWindow& mWindow;
    std::vector<std::vector<sf::RectangleShape>> mFieldRects;
};
