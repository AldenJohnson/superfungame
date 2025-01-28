#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>  // Add SFML graphics header

class Player {
public:
    Player(const sf::Texture& texture, float speed);  // Constructor declaration
    void update(sf::Time deltaTime);                  // Update function
    void render(sf::RenderWindow& window);            // Render function
    void handleInput();                               // Handle input (movement and animation)
    sf::Sprite& getSprite();                          // Get sprite

private:
    sf::Sprite sprite;           // Sprite for the player
    sf::Vector2f position;       // Player's position
    sf::Vector2f velocity;       // Player's velocity
    float speed;                 // Movement speed
    int animationRow;            // Current animation row
    int currentFrame;            // Current animation frame
    sf::Clock animationClock;    // Animation clock
};

#endif
