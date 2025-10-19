#include <SFML/Graphics.hpp>

class Game {
   public:
    Game();

    void run();

    const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
    const float PlayerSpeed = 80.f;

   private:
    void processEvents();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow mWindow;
    sf::Texture mTexture;
    sf::Sprite mPlayer;

    bool mIsMovingUp = false;
    bool mIsMovingDown = false;
    bool mIsMovingLeft = false;
    bool mIsMovingRight = false;
};
