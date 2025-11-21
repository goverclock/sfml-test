#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../include/mine_cell.hpp"

struct MineCellUI {
    MineCellUI() : rect({cell_size, cell_size}), mine_number_text({}) {};
    constexpr static float cell_size = 80.f;
    sf::RectangleShape rect;
    std::optional<sf::Text> mine_number_text;
};

class LocalUI {
   public:
    LocalUI(sf::RenderWindow& w)
        : mWindow(w), mFont("resource/JetBrainsMono-Regular.ttf") {};
    void init_field(const size_t row, const size_t col);
    void render_field(const MineField& mine_field);
    std::optional<sf::Vector2i> get_rol_col_by_pos(const sf::Vector2i pos);

   private:
    sf::RenderWindow& mWindow;
    std::vector<std::vector<MineCellUI>> mFieldUI;
    sf::Font mFont;
};
