#include "entity.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void Entity::setVelocity(sf::Vector2f velocity) { mVelocity = velocity; }

void Entity::setVelocity(float vx, float vy) {
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const { return mVelocity; }

Aircraft::Aircraft(Aircraft::Type type, const TextureHolder& textures)
    : mType(type), mSprite(textures.get(Textures::ID::Eagle)) {}

void Aircraft::drawCurrent(sf::RenderTarget& target,
                           sf::RenderStates states) const {
    target.draw(mSprite, states);
}

Textures::ID Aircraft::toTextureID(Aircraft::Type type) {
    switch (type) {
        case Aircraft::Type::Eagle:
            return Textures::ID::Eagle;
            break;
        case Aircraft::Type::Raptor:
            return Textures::ID::Raptor;
            break;
    }
    assert(false);
}