#include "Player.hpp"
#include <SFML/Graphics.hpp>

Player::Player(const sf::Texture& texture, float speed)
    : position(400.f, 300.f), velocity(0.f, 0.f), speed(speed), animationRow(0), currentFrame(0) { // Correct order
    sprite.setTexture(texture);
    sprite.setOrigin(64, 64); // Assuming 128x128 sprite size
    sprite.setPosition(position);
}

void Player::update(sf::Time deltaTime) {
    // Update the player position based on velocity
    position += velocity * deltaTime.asSeconds();
    sprite.setPosition(position);

    // Update animation frame if needed
    if (velocity.x == 0.f && velocity.y == 0.f) {
        currentFrame = 0; // Idle frame
    } else if (animationClock.getElapsedTime().asSeconds() > 0.15f) {
        currentFrame = (currentFrame + 1) % 4; // Cycle through frames
        animationClock.restart();
    }

    // Set the texture rect for the player sprite based on the animation
    sprite.setTextureRect(sf::IntRect(currentFrame * 128, animationRow * 128, 128, 128));
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);  // Draw the player sprite
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        velocity.y = -speed;
        animationRow = 1; // Up (Row 2)
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        velocity.y = speed;
        animationRow = 0; // Down (Row 1)
    } else {
        velocity.y = 0.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x = -speed;
        animationRow = 2; // Left (Row 3)
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x = speed;
        animationRow = 3; // Right (Row 4)
    } else {
        velocity.x = 0.f;
    }
}

sf::Sprite& Player::getSprite() {
    return sprite; // Provide access to the sprite
}
