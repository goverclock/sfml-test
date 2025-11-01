#include "entity.hpp"
#include <SFML/Graphics/RenderTarget.hpp>

void Entity::setVelocity(sf::Vector2f velocity) { mVelocity = velocity; }

void Entity::setVelocity(float vx, float vy) {
    mVelocity.x = vx;
    mVelocity.y = vy;
}

sf::Vector2f Entity::getVelocity() const { return mVelocity; }

Aircraft::Aircraft(Aircraft::Type type) : mType(type) {
    
}

void Aircraft::drawCurrent(sf::RenderTarget& target,
                           sf::RenderStates states) const {
    target.draw(mSprite, states);
}
