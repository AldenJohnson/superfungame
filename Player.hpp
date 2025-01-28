#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>  // Include SFML for graphics rendering

class Player {
public:
    Player(const sf::Texture& texture, float speed);  // Constructor declaration

    void update(sf::Time deltaTime);                  // Update function (movement and animation)
    void render(sf::RenderWindow& window);            // Render function (draws the player sprite)
    void handleInput();                               // Handle input (WASD movement and animation)

    const sf::Vector2f& getPosition() const;          // Returns the player's current position
    int getAnimationRow() const;                      // Returns the current animation row
    sf::Sprite& getSprite();                          // Returns the player's sprite

private:
    sf::Sprite sprite;           // The player's sprite (visual representation)
    sf::Vector2f position;       // The player's position on the map
    sf::Vector2f velocity;       // The player's movement velocity
    float speed;                 // The player's movement speed
    int animationRow;            // The current row in the spritesheet (direction)
    int currentFrame;            // The current animation frame
    sf::Clock animationClock;    // Tracks time for animation frame updates
};

#endif // PLAYER_HPP