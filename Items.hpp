#ifndef ITEMS_H
#define ITEMS_H

#include <SFML/Graphics.hpp>

class Arrow {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;

    Arrow(float radius = 5.0f); // Constructor
    void update(float deltaTime); // Updates the arrow's position
};

#endif // ITEMS_H
