#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

#include "../include/local_status.hpp"
#include "../include/local_ui.hpp"

int main() {
    sf::RenderWindow w(sf::VideoMode({1920, 1080}), "FUCK BOOK");
    LocalStatus local_status;
    LocalUI local_ui(local_status);
    // local_status.start_game(10, 10);

    const auto on_mouse_button_pressed =
        [&](const sf::Event::MouseButtonPressed& mouse_button_pressed) {
            // TODO: should only pass mouse relevant event
            local_ui.handle_click_event(w, mouse_button_pressed);
        };
    const auto on_key_pressed = [&](const sf::Event::KeyPressed& key_pressed) {
        if (key_pressed.scancode == sf::Keyboard::Scancode::Q) w.close();
    };

    while (w.isOpen()) {
        w.handleEvents([&](const sf::Event::Closed) { w.close(); },
                       on_key_pressed, on_mouse_button_pressed);
        w.clear(sf::Color(55, 55, 55, 0));
        local_ui.render(w);
        w.display();
    }

    return 0;
}
