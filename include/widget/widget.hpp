#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace widget {

static const sf::Font defaultWidgetFont("resource/JetBrainsMono-Regular.ttf");

struct Responsive {
    virtual bool handle_event(sf::RenderWindow& w, sf::Event e) = 0;
};

struct Widget : public sf::Transformable,
                public sf::Drawable,
                public Responsive {};

};  // namespace widget
