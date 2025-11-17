#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

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
        // for ()
        // for (const auto& r : rr) {
        // const sf::FloatRect bound = r.getGlobalBounds();
        // if (bound.contains({(float)mouse_pos.x, (float)mouse_pos.y}))
        //     std::cout << "clicked on: " << << std::endl;
        // }
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

    sf::RectangleShape rect({100.f, 100.f});
    std::vector<std::vector<sf::RectangleShape>> rects;
    for (int i = 0; i < 10; i++) {
        rects.emplace_back(std::vector<sf::RectangleShape>(
            10, sf::RectangleShape({100.f, 100.f})));
        for (int j = 0; j < 10; j++) {
            rects[i][j].setPosition({10.f + 110.f * i, 10.f + 110.f * j});
        }
    }

    // for (const auto& rr : rects)
    //     for (const auto& r : rr) {
    //         const sf::FloatRect bound = r.getGlobalBounds();
    //         std::cout << bound.position.x << " " << bound.position.y
    //                   << std::endl;
    //         std::cout << bound.size.x << " " << bound.size.y << std::endl;
    //     }

    while (w.isOpen()) {
        w.handleEvents(
            [&](const sf::Event::KeyPressed& key_pressed) {
                on_key_pressed(w, key_pressed);
            },
            [&](const sf::Event::Closed) { w.close(); },
            [&](const sf::Event::MouseButtonPressed& mouse_button_pressed) {
                on_mouse_button_pressed(w, mouse_button_pressed, rects);
            });

        // render
        // try drawing some rectangles first
        w.clear(sf::Color(55, 55, 55, 0));

        for (const auto& rr : rects)
            for (const auto& r : rr) w.draw(r);

        w.display();
    }

    return 0;
}
