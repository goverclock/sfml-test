#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "scene.hpp"
#include "resource_holder.hpp"

class Entity : public SceneNode {
   public:
    void setVelocity(sf::Vector2f velocity);
    void setVelocity(float vx, float vy);
    sf::Vector2f getVelocity() const;

   private:
    sf::Vector2f mVelocity;
};

class Aircraft : public Entity {
   public:
    enum Type {
        Eagle,
        Raptor,
    };

   public:
    explicit Aircraft(Type type, const TextureHolder& textures);
    Aircraft() = delete;
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const override;

   private:
    static Textures::ID toTextureID(Aircraft::Type type);
    Type mType;
    sf::Sprite mSprite;
};
