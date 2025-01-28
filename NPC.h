#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>

class NPC {
public:
    NPC(const sf::Texture& texture, float speed);
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);
    sf::Sprite& getSprite();
    
private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    int animationRow;
    int currentFrame;
    sf::Clock animationClock;
    sf::Clock movementClock;
};

#endif // NPC_H