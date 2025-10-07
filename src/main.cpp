#include <SFML/Graphics.hpp>

class Game {
   public:
    Game();

    void run();

   private:
    void processEvents();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void update();
    void render();

    sf::RenderWindow mWindow;
    sf::CircleShape mPlayer;

    bool mIsMovingUp = false;
    bool mIsMovingDown = false;
    bool mIsMovingLeft = false;
    bool mIsMovingRight = false;
};

Game::Game()
    : mWindow(sf::VideoMode({640u, 480u}), "SFML Application"), mPlayer() {
    mPlayer.setRadius(40.f);
    mPlayer.setPosition({100.f, 100.f});
    mPlayer.setFillColor(sf::Color::Blue);
}

void Game::processEvents() {
    while (const std::optional e = mWindow.pollEvent()) {
        if (e->is<sf::Event::KeyPressed>()) {
            handlePlayerInput(e->getIf<sf::Event::KeyPressed>()->code, true);
        } else if (e->is<sf::Event::KeyReleased>()) {
            handlePlayerInput(e->getIf<sf::Event::KeyReleased>()->code, false);
        } else if (e->is<sf::Event::Closed>())
            mWindow.close();
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    switch (key) {
        case sf::Keyboard::Key::W:
            mIsMovingUp = isPressed;
            break;
        case sf::Keyboard::Key::S:
            mIsMovingDown = isPressed;
            break;
        case sf::Keyboard::Key::A:
            mIsMovingLeft = isPressed;
            break;
        case sf::Keyboard::Key::D:
            mIsMovingRight = isPressed;
            break;
        default:
            break;
    }
}

void Game::update() {
    sf::Vector2f movement(0.f, 0.f);
    if (mIsMovingUp) movement.y -= .1f;
    if (mIsMovingDown) movement.y += .1f;
    if (mIsMovingLeft) movement.x -= .1f;
    if (mIsMovingRight) movement.x += .1f;

    mPlayer.move(movement);
}

void Game::render() {
    // if (!updated) return;
    // updated = false;
    // printf("R\n");

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
