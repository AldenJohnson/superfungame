#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <vector>

enum TileType { GRASS, WATER };

std::vector<std::vector<TileType>> generateWorld();
void renderWorld(sf::RenderWindow& window, const std::vector<std::vector<TileType>>& map, sf::Sprite& grassSprite, sf::Sprite& waterSprite);

#endif // WORLD_H
