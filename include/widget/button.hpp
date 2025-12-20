#pragma once
#include "renderable.hpp"

namespace widget {

class Button : public Renderable, public sf::Transform, public sf::Drawable {
   public:
    Button(float width = 100, float height = 100,
           std::string text = "unnamed button");

    bool is_clicked();
    void set_position(sf::Vector2f pos);
    void set_size(sf::Vector2f size);
    void set_fill_color(sf::Color color);

    // Renderable
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

    // Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
    bool mIsClicked = false;
    sf::RectangleShape mRectangle;
    sf::Text mText;
    sf::Font mFont;
};

};  // namespace widget
