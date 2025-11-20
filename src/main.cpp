#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "../include/local_status.hpp"

void on_key_pressed(sf::RenderWindow& w,
                    const sf::Event::KeyPressed& key_pressed) {
    if (key_pressed.scancode == sf::Keyboard::Scancode::Q) w.close();
}

void on_mouse_button_pressed(
    sf::RenderWindow& w,
    const sf::Event::MouseButtonPressed& mouse_button_pressed,
    const std::vector<std::vector<sf::RectangleShape>>& rects) {
    if (mouse_button_pressed.button == sf::Mouse::Button::Left) {
        const auto mouse_pos = sf::Mouse::getPosition(w);
        std::cout << "mouse at: " << mouse_pos.x << " " << mouse_pos.y
                  << std::endl;
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++) {
                const sf::FloatRect bound = rects[i][j].getGlobalBounds();
                if (bound.contains({(float)mouse_pos.x, (float)mouse_pos.y}))
                    std::cout << "clicked on: " << i << " " << j << std::endl;
            }
    }
}

int main() {
    sf::RenderWindow w(sf::VideoMode({1920, 1080}), "FUCK BOOK");
    LocalStatus local_status;
    local_status.start_game(10, 20);

    sf::RectangleShape covered_style_rect({100.f, 100.f});
    covered_style_rect.setFillColor(sf::Color::Black);
    sf::RectangleShape revealed_style_rect({100.f, 100.f});
    revealed_style_rect.setFillColor(sf::Color::White);

    while (w.isOpen()) {
        w.handleEvents(
            [&](const sf::Event::KeyPressed& key_pressed) {
                on_key_pressed(w, key_pressed);
            }
            // [&](const sf::Event::Closed) { w.close(); }[&](
            //     const sf::Event::MouseButtonPressed& mouse_button_pressed,
            //     const LocalStatus& local_status) {
            //     on_mouse_button_pressed(w, mouse_button_pressed, rects);
            // }
        );

        w.clear(sf::Color(55, 55, 55, 0));

        for (size_t r = 0; r < local_status.field_rows(); r++)
            for (size_t c = 0; c < local_status.field_cols(); c++) {
                const MineCell& cell_status = local_status.get_cell(r, c);
                switch (cell_status.mCellStatus) {
                    case CellStatus::Covered:
                        covered_style_rect.setPosition({110.f * r, 110.f * c});
                        w.draw(covered_style_rect);
                        break;
                    case CellStatus::Revealed:
                        revealed_style_rect.setPosition({110.f * r, 110.f * c});
                        w.draw(revealed_style_rect);
                        break;
                    case CellStatus::Marked:
                        assert(false && "not implemented");
                        break;
                }
            }

        w.display();
    }

    return 0;
}
