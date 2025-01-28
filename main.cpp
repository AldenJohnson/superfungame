#include <SFML/Graphics.hpp>
#include "bullet.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

// Constants for map dimensions and tile size
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 10;
const int TILE_SIZE = 128; // Base tile size (128x128)

// Tile types
enum TileType { GRASS, WATER };

// Container for active bullets
std::vector<Bullet> bullets;

// Function to generate the world map
std::vector<std::vector<TileType>> generateWorld() {
    std::vector<std::vector<TileType>> map(MAP_HEIGHT, std::vector<TileType>(MAP_WIDTH));
    srand(static_cast<unsigned>(time(0)));

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileType = rand() % 2; // Randomly select a tile type (0 = GRASS, 1 = WATER)
            map[y][x] = static_cast<TileType>(tileType);
        }
    }
    return map;
}

// Function to render the world map (tiles)
void renderWorld(
    sf::RenderWindow& window,
    const std::vector<std::vector<TileType>>& map,
    sf::Sprite& grassSprite,
    sf::Sprite& waterSprite
) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            sf::Sprite* spriteToDraw = nullptr;
            switch (map[y][x]) {
                case GRASS: spriteToDraw = &grassSprite; break;
                case WATER: spriteToDraw = &waterSprite; break;
            }
            if (spriteToDraw) {
                spriteToDraw->setPosition(x * TILE_SIZE, y * TILE_SIZE);
                window.draw(*spriteToDraw);
            }
        }
    }
}

// Inventory parameters
const int inventoryBoxSize = 64;  // Size of each inventory box (64x64)
const int inventoryColumns = 4;   // Number of columns for inventory boxes
const int inventoryPadding = 10;  // Padding between inventory boxes

// Simple inventory data
std::vector<sf::RectangleShape> inventory;

void setupInventory() {
    for (int i = 0; i < inventoryColumns; ++i) {
        sf::RectangleShape box(sf::Vector2f(inventoryBoxSize, inventoryBoxSize));
        box.setPosition(inventoryPadding + i * (inventoryBoxSize + inventoryPadding),
                        600 - inventoryBoxSize - inventoryPadding); // Bottom of the screen
        box.setFillColor(sf::Color(150, 150, 150)); // Gray color
        inventory.push_back(box);
    }
}

void drawInventory(sf::RenderWindow& window, sf::View& hudView) {
    window.setView(hudView); // Use HUD view
    for (const auto& box : inventory) {
        window.draw(box);
    }
}

int main() {
    // Create the game window (resizable)
    sf::RenderWindow window(sf::VideoMode(800, 600), "Player and NPC Animations", sf::Style::Resize);

    // Load textures for tiles and player/NPC
    sf::Texture grassTexture, waterTexture, playerTexture, npcTexture;
    if (!grassTexture.loadFromFile("grass.png") ||
        !waterTexture.loadFromFile("water.png") ||
        !playerTexture.loadFromFile("player_spritesheet.png") ||
        !npcTexture.loadFromFile("npc_spritesheet.png")) { // NPC spritesheet
        std::cerr << "Error: Failed to load textures." << std::endl;
        return -1;
    }

    // Create sprites for tiles
    sf::Sprite grassSprite(grassTexture);
    sf::Sprite waterSprite(waterTexture);

    // Player setup
    sf::Sprite playerSprite(playerTexture);
    playerSprite.setOrigin(TILE_SIZE / 2, TILE_SIZE / 2); // Center origin
    sf::Vector2f playerPosition(400.f, 300.f);           // Initial player position
    sf::Vector2f velocity(0.f, 0.f);                     // Initial velocity
    float speed = 200.0f;                                // Movement speed

    // NPC setup
    sf::Sprite npcSprite(npcTexture);
    npcSprite.setOrigin(TILE_SIZE / 2, TILE_SIZE / 2); // Center origin
    sf::Vector2f npcPosition(100.f, 100.f);           // NPC position
    sf::Vector2f npcVelocity(0.f, 0.f);              // NPC velocity
    sf::Clock npcMovementClock, npcAnimationClock;   // Timers for NPC movement and animation
    const float npcMoveInterval = 2.0f;              // Time between direction changes
    int npcCurrentFrame = 0, npcAnimationRow = 1;    // NPC animation state

    // Animation setup
    sf::Clock playerAnimationClock, movementClock;
    int playerCurrentFrame = 0, playerAnimationRow = 1;

    // Generate the world map
    std::vector<std::vector<TileType>> world = generateWorld();

    // Camera setup
    sf::View gameView(sf::FloatRect(0, 0, 800, 600));
    sf::View hudView(sf::FloatRect(0, 0, 800, 600));

    // Setup inventory
    setupInventory();

    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Update delta time
        sf::Time deltaTime = movementClock.restart();

        // Player movement logic (WASD)
        velocity = {0.f, 0.f};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) velocity.y = -speed, playerAnimationRow = 2;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) velocity.y = speed, playerAnimationRow = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) velocity.x = -speed, playerAnimationRow = 3;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) velocity.x = speed, playerAnimationRow = 4;

        // Update player position
        playerPosition += velocity * deltaTime.asSeconds();

        // Player animation
        if (velocity == sf::Vector2f(0.f, 0.f)) playerCurrentFrame = 0;
        else if (playerAnimationClock.getElapsedTime().asSeconds() > 0.15f) {
            playerCurrentFrame = (playerCurrentFrame + 1) % 4;
            playerAnimationClock.restart();
        }

        sf::IntRect playerFrame(playerCurrentFrame * TILE_SIZE, (playerAnimationRow - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        playerSprite.setTextureRect(playerFrame);

        // NPC movement and animation logic
        if (npcMovementClock.getElapsedTime().asSeconds() > npcMoveInterval) {
            int randomDirection = rand() % 4;
            float npcSpeed = 50.0f;

            if (randomDirection == 0) npcVelocity = {0.f, -npcSpeed}, npcAnimationRow = 2;
            else if (randomDirection == 1) npcVelocity = {0.f, npcSpeed}, npcAnimationRow = 1;
            else if (randomDirection == 2) npcVelocity = {-npcSpeed, 0.f}, npcAnimationRow = 3;
            else npcVelocity = {npcSpeed, 0.f}, npcAnimationRow = 4;

            npcMovementClock.restart();
        }

        npcPosition += npcVelocity * deltaTime.asSeconds();

        if (npcVelocity != sf::Vector2f(0.f, 0.f) && npcAnimationClock.getElapsedTime().asSeconds() > 0.15f) {
            npcCurrentFrame = (npcCurrentFrame + 1) % 4;
            npcAnimationClock.restart();
        } else if (npcVelocity == sf::Vector2f(0.f, 0.f)) npcCurrentFrame = 0;

        sf::IntRect npcFrame(npcCurrentFrame * TILE_SIZE, (npcAnimationRow - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        npcSprite.setTextureRect(npcFrame);
        npcSprite.setPosition(npcPosition);

        // Handle shooting (Space key)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            static sf::Clock shootClock;
            if (shootClock.getElapsedTime().asSeconds() > 0.2f) {
                Bullet bullet;
                bullet.shape.setPosition(playerPosition);

                // Determine bullet direction based on player animation row
                if (playerAnimationRow == 1)       // Down
                    bullet.velocity = sf::Vector2f(0.f, 1.f);
                else if (playerAnimationRow == 2)  // Up
                    bullet.velocity = sf::Vector2f(0.f, -1.f);
                else if (playerAnimationRow == 3)  // Left
                    bullet.velocity = sf::Vector2f(-1.f, 0.f);
                else if (playerAnimationRow == 4)  // Right
                    bullet.velocity = sf::Vector2f(1.f, 0.f);

                bullets.push_back(bullet);
                shootClock.restart();
            }
        }

        // Update bullets
        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i].update(deltaTime.asSeconds());
            if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > MAP_WIDTH * TILE_SIZE ||
                bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > MAP_HEIGHT * TILE_SIZE) {
                bullets.erase(bullets.begin() + i);
                --i;
            }
        }

        // Update camera
        gameView.setCenter(playerPosition);
        window.setView(gameView);

        // Render everything
        window.clear();
        renderWorld(window, world, grassSprite, waterSprite);
        playerSprite.setPosition(playerPosition);
        window.draw(playerSprite);
        window.draw(npcSprite);
        for (const auto& bullet : bullets) window.draw(bullet.shape);
        drawInventory(window, hudView);
        window.display();
    }

    return 0;
}