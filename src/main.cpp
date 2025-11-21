#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include "../include/local_status.hpp"
#include "../include/local_ui.hpp"

int main() {
    sf::RenderWindow w(sf::VideoMode({1920, 1080}), "FUCK BOOK");
    LocalUI local_ui(w);
    LocalStatus local_status;
    local_status.start_game(10, 2);
    local_ui.init_field(10, 2);

    const auto on_mouse_button_pressed =
        [&](const sf::Event::MouseButtonPressed& mouse_button_pressed) {
            if (mouse_button_pressed.button == sf::Mouse::Button::Left) {
                const auto mouse_pos = sf::Mouse::getPosition(w);
                std::cout << "mouse at: " << mouse_pos.x << " " << mouse_pos.y
                          << std::endl;
                const auto rol_col = local_ui.get_rol_col_by_pos(mouse_pos);
                if (rol_col) {
                    std::cout << "clicking row col: " << rol_col->x << " "
                              << rol_col->y << std::endl;
                    local_status.reveal(rol_col->x, rol_col->y);
                } else
                    std::cout << "not clicking on any cell" << std::endl;
            }
        };
    const auto on_key_pressed = [&](const sf::Event::KeyPressed& key_pressed) {
        if (key_pressed.scancode == sf::Keyboard::Scancode::Q) w.close();
    };

    while (w.isOpen()) {
        w.handleEvents([&](const sf::Event::Closed) { w.close(); },
                       on_key_pressed, on_mouse_button_pressed);

        w.clear(sf::Color(55, 55, 55, 0));

        local_ui.render_field(local_status.get_field());

        w.display();
    }

    return 0;
}
