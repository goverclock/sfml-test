#include "game_client.hpp"

void GameClient::run() {
    static const auto on_mouse_button_pressed =
        [&](const sf::Event::MouseButtonPressed& mouse_button_pressed) {
            // TODO: should only pass mouse relevant event
            mLocalUI.handle_click_event(mWindow, mouse_button_pressed);
        };
    static const auto on_key_pressed =
        [&](const sf::Event::KeyPressed& key_pressed) {
            if (key_pressed.scancode == sf::Keyboard::Scancode::Q)
                mWindow.close();
        };

    while (mWindow.isOpen()) {
        mWindow.handleEvents([&](const sf::Event::Closed) { mWindow.close(); },
                             on_key_pressed, on_mouse_button_pressed);
        mWindow.clear(sf::Color(55, 55, 55, 0));
        mLocalUI.render(mWindow);
        mWindow.display();
    }
}
