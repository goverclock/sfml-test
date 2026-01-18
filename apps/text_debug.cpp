#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <print>

#include "scene_manager.hpp"
#include "widget/button.hpp"

static sf::Font fuckingFont("resource/JetBrainsMono-Regular.ttf");

struct MyBtn : public sf::Drawable {
    MyBtn()
        : mFont("resource/JetBrainsMono-Regular.ttf"), mText(mFont, "FUCK") {
        mText.setFillColor(sf::Color::White);
        mText.setCharacterSize(50);
    };

    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override {
        target.draw(mText, states);
    }

    sf::Text mText;
    sf::Font mFont;
};

void push_btn(std::vector<MyBtn>& btns) {
    MyBtn btn;
    btns.push_back(btn);
}

int main() {
    std::vector<MyBtn> btns;
    push_btn(btns);

    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }
        window.clear(sf::Color::Black);

        window.draw(btns[0]);

        // window.draw(txts[1]);
        window.display();
    }

    return 0;
}
