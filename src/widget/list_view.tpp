#include "widget/list_view.hpp"

namespace widget {

template <HasToString T>
void ListView<T>::render(sf::RenderWindow& w) {
    w.draw(*this);
}

template <HasToString T>
void ListView<T>::handle_click_event(sf::RenderWindow& w, sf::Event e) {}

template <HasToString T>
void ListView<T>::draw(sf::RenderTarget& target,
                       sf::RenderStates states) const {
    states.transform *= getTransform();

    sf::RectangleShape entry_rect;
    entry_rect.setSize({400.f, 80.f});
    entry_rect.setFillColor(sf::Color::White);
    sf::Font font("resource/JetBrainsMono-Regular.ttf");
    sf::Text entry_text(font);
    entry_text.setFillColor(sf::Color::Black);
    entry_text.setCharacterSize(50);

    float y = 0.f;
    for (const T& e : list) {
        entry_rect.setPosition({0, y});
        entry_text.setPosition({0, y});
        entry_text.setString(e.to_string());
        target.draw(entry_rect, states);
        target.draw(entry_text, states);

        y += entry_rect.getSize().y;
    }
}

};  // namespace widget
