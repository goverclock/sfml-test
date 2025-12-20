#include "ergonomics.hpp"
#include "widget/button.hpp"

namespace widget {

Button::Button(float width, float height, std::string text)
    : mFont("resource/JetBrainsMono-Regular.ttf"), mText(mFont, text) {
    mText.setFillColor(sf::Color::Black);
    mText.setCharacterSize(50);
};

// note: should only be called right after handle_click_event, updated on each
// handle_click_event call
bool Button::is_clicked() { TODO(); }

void Button::set_position(sf::Vector2f pos) { mRectangle.setPosition(pos); }

void Button::set_size(sf::Vector2f size) { mRectangle.setSize(size); }

void Button::set_fill_color(sf::Color color) { mRectangle.setFillColor(color); }

void Button::render(sf::RenderWindow& w) { TODO(); }

void Button::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    TODO();
    mIsClicked = false;
    // check if the rectangle is clicked on
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    TODO();
}

};  // namespace widget
