#ifndef ITEMS_H
#define ITEMS_H

#include <SFML/Graphics.hpp>

class Football {
public:
    Football(const sf::Texture& texture);
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);
    void throwFootball(const sf::Vector2f& playerPosition, const sf::Vector2f& npcPosition);
    bool isThrown() const;

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f velocity;
    bool thrown;
    sf::Clock throwTimer;
};

#endif // ITEMS_H
