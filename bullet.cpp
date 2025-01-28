#include "bullet.hpp"

Bullet::Bullet(float radius) {
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(radius, radius); // Center the origin
    speed = 500.0f; // Bullet speed
    velocity = sf::Vector2f(0.f, 0.f); // Default velocity
}

void Bullet::update(float deltaTime) {
    shape.move(velocity * speed * deltaTime); // Move the bullet
}