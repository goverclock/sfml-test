#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

struct Renderable {
    virtual ~Renderable() {};
    virtual void handle_click_event(sf::RenderWindow& w, sf::Event e) = 0;
};
