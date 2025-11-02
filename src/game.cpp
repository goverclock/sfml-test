#include "game.hpp"

Game::Game()
    : mWindow(sf::VideoMode({1920u, 1080u}), "SFML Application"),
      mTexture("assets/banio.jpg"),
      mPlayer(mTexture) {}

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

void Game::update(sf::Time deltaTime) {
    sf::Vector2f movement(0.f, 0.f);
    if (mIsMovingUp) movement.y -= PlayerSpeed;
    if (mIsMovingDown) movement.y += PlayerSpeed;
    if (mIsMovingLeft) movement.x -= PlayerSpeed;
    if (mIsMovingRight) movement.x += PlayerSpeed;

    mPlayer.move(movement * deltaTime.asSeconds());
}

void Game::render() {
    sf::Texture t("assets/banio.jpg");
    sf::Sprite s(t);

    mWindow.clear();
    mWindow.draw(mPlayer);
    mWindow.display();
}

void Game::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen()) {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processEvents();
            update(TimePerFrame);
        }
        render();
    }
}
