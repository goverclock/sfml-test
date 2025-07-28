#include <SFML/Graphics.hpp>

class Game {
   public:
    Game();

    void run();

   private:
    void processEvents();
    void update();
    void render();

    bool updated = true;
    float x = 100.f;
    float y = 100.f;

    sf::RenderWindow mWindow;
    sf::CircleShape mPlayer;
};

Game::Game()
    : mWindow(sf::VideoMode({640u, 480u}), "SFML Application"), mPlayer() {
    mPlayer.setRadius(40.f);
    mPlayer.setPosition({100.f, 100.f});
    mPlayer.setFillColor(sf::Color::Blue);
}

void Game::processEvents() {
    while (const std::optional e = mWindow.pollEvent()) {
        if (e->is<sf::Event::Closed>()) mWindow.close();
        if (e->is<sf::Event::KeyPressed>()) {
            switch (e->getIf<sf::Event::KeyPressed>()->code) {
                case sf::Keyboard::Key::A:
                    x -= 10;
                    break;
                case sf::Keyboard::Key::D:
                    x += 10;
                    break;
                case sf::Keyboard::Key::W:
                    y -= 10;
                    break;
                case sf::Keyboard::Key::S:
                    y += 10;
                    break;
                default:
                    break;
            }
        }
        updated = true;
    }
}

void Game::update() { mPlayer.setPosition({x, y}); }

void Game::render() {
    if (!updated) return;
    updated = false;
    printf("R\n");

    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.display();
}

void Game::run() {
    while (mWindow.isOpen()) {
        processEvents();
        update();
        render();
    }
}

int main() {
    Game g;
    g.run();

    return 0;
}
