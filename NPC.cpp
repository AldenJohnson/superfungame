#include "NPC.h"
#include <cstdlib>
#include <cmath>

NPC::NPC(const sf::Texture& texture, float speed) 
    : speed(speed), animationRow(1), currentFrame(0) {
    sprite.setTexture(texture);
    sprite.setOrigin(64, 64);  // Assuming sprite size is 128x128
    position = sf::Vector2f(100.f, 100.f);
    sprite.setPosition(position);
}

void NPC::update(sf::Time deltaTime) {
    if (movementClock.getElapsedTime().asSeconds() > 2.f) {
        int randomDirection = rand() % 4;  // 0 = up, 1 = down, 2 = left, 3 = right
        if (randomDirection == 0) {
            velocity = sf::Vector2f(0.f, -speed);
            animationRow = 2;
        } else if (randomDirection == 1) {
            velocity = sf::Vector2f(0.f, speed);
            animationRow = 1;
        } else if (randomDirection == 2) {
            velocity = sf::Vector2f(-speed, 0.f);
            animationRow = 3;
        } else {
            velocity = sf::Vector2f(speed, 0.f);
            animationRow = 4;
        }
        movementClock.restart();
    }
    position += velocity * deltaTime.asSeconds();
    sprite.setPosition(position);

    if (animationClock.getElapsedTime().asSeconds() > 0.15f) {
        currentFrame = (currentFrame + 1) % 4;
        animationClock.restart();
    }
}

void NPC::render(sf::RenderWindow& window) {
    sf::IntRect frame(currentFrame * 128, (animationRow - 1) * 128, 128, 128);
    sprite.setTextureRect(frame);
    window.draw(sprite);
}

sf::Sprite& NPC::getSprite() {
    return sprite;
}
