#ifndef BULLET_HPP
#define BULLET_HPP

#include <SFML/Graphics.hpp>

class Bullet {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;

    Bullet(float radius = 5.0f); // Constructor
    void update(float deltaTime); // Updates the bullet's position
};

#endif