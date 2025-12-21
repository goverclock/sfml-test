#pragma once
#include <functional>

#include "renderable.hpp"

namespace widget {

class Button : public Renderable,
               public sf::Transformable,
               public sf::Drawable {
   public:
    Button(float width = 100, float height = 100,
           std::string text = "unnamed button");

    void on_click(const std::function<void(void)>& callback_func);
    void set_position(sf::Vector2f pos);
    void set_size(sf::Vector2f size);
    sf::Vector2f get_size() const;
    void set_fill_color(sf::Color color);
    void set_text(std::string text);

    // Renderable
    void render(sf::RenderWindow& w) override;
    void handle_click_event(sf::RenderWindow& w, sf::Event e) override;

    // Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

   private:
    std::function<void(void)> mClickCallbackFunc;
    sf::RectangleShape mRectangle;
    sf::Text mText;
    // TODO: refactor into a ResourceManager to store mFont for all class
    sf::Font mFont;
};

};  // namespace widget
