#include "ergonomics.hpp"
#include "widget/button.hpp"

namespace widget {

Button::Button(float width, float height, std::string text)
    : mFont("resource/JetBrainsMono-Regular.ttf"), mText(mFont, text) {
    mRectangle.setFillColor(sf::Color::White);
    mText.setFillColor(sf::Color::Black);
    mText.setCharacterSize(50);
};

void Button::on_click(const std::function<void(void)>& callback_func) {
    mClickCallbackFunc = callback_func;
}

void Button::set_position(sf::Vector2f pos) {
    mRectangle.setPosition(pos);
    mText.setPosition(pos);
}

void Button::set_size(sf::Vector2f size) { mRectangle.setSize(size); }

sf::Vector2f Button::get_size() const { return mRectangle.getSize(); }

void Button::set_fill_color(sf::Color color) { mRectangle.setFillColor(color); }

void Button::set_text(std::string text) { mText.setString(text); }

void Button::render(sf::RenderWindow& w) { w.draw(*this); }

void Button::handle_click_event(sf::RenderWindow& w, sf::Event e) {
    const sf::Event::MouseButtonPressed* mouse_button_pressed =
        e.getIf<sf::Event::MouseButtonPressed>();
    assert(mouse_button_pressed);
    const auto mouse_pos = sf::Mouse::getPosition(w);

    if (mRectangle.getGlobalBounds().contains(
            {(float)mouse_pos.x, (float)mouse_pos.y})) {
        std::println("Button({}) clicked",
                     static_cast<std::string>(mText.getString()));
        mClickCallbackFunc();
    }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mRectangle);
    target.draw(mText);
}

};  // namespace widget
