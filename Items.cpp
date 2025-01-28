#include "Items.hpp"
#include <cmath>

Football::Football(const sf::Texture& texture)
    : thrown(false) {
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
}

void Football::throwFootball(const sf::Vector2f& playerPosition, const sf::Vector2f& npcPosition) {
    position = playerPosition;
    sf::Vector2f direction = npcPosition - playerPosition;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction /= length;  // Normalize direction
    velocity = direction * 300.f;  // Speed of throw
    thrown = true;
    throwTimer.restart();
}

void Football::update(sf::Time deltaTime) {
    if (thrown) {
        position += velocity * deltaTime.asSeconds();
    }
}

void Football::render(sf::RenderWindow& window) {
    if (thrown) {
        sprite.setPosition(position);
        window.draw(sprite);
    }
}

bool Football::isThrown() const {
    return thrown;
}
