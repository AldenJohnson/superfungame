#include "World.hpp"
#include <cstdlib>
#include <ctime>

std::vector<std::vector<TileType>> generateWorld() {
    std::vector<std::vector<TileType>> map(10, std::vector<TileType>(20));
    srand(static_cast<unsigned>(time(0)));

    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 20; ++x) {
            map[y][x] = static_cast<TileType>(rand() % 2);  // Randomly select a tile (0 = GRASS, 1 = WATER)
        }
    }
    return map;
}

void renderWorld(sf::RenderWindow& window, const std::vector<std::vector<TileType>>& map, sf::Sprite& grassSprite, sf::Sprite& waterSprite) {
    for (int y = 0; y < 10; ++y) {
        for (int x = 0; x < 20; ++x) {
            sf::Sprite* spriteToDraw = nullptr;
            switch (map[y][x]) {
                case GRASS: spriteToDraw = &grassSprite; break;
                case WATER: spriteToDraw = &waterSprite; break;
            }
            if (spriteToDraw) {
                spriteToDraw->setPosition(x * 128, y * 128);
                window.draw(*spriteToDraw);
            }
        }
    }
}
