#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

// Constants for map dimensions and tile size
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 10;
const int TILE_SIZE = 128; // Base tile size (128x128)

// Tile types
enum TileType { GRASS, WATER };

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
    // Create inventory boxes
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
    // Create the game window (resizeable)
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
    playerSprite.setOrigin(TILE_SIZE / 2, TILE_SIZE / 2); // Set the origin to the center of the sprite
    sf::Vector2f playerPosition(400.f, 300.f); // Initial player position (center of the window)
    sf::Vector2f velocity(0.f, 0.f); // Initial velocity
    float speed = 200.0f; // Movement speed

    // NPC setup
    sf::Sprite npcSprite(npcTexture);
    npcSprite.setOrigin(TILE_SIZE / 2, TILE_SIZE / 2); // Set the origin to the center of the sprite
    sf::Vector2f npcPosition(100.f, 100.f); // Initial NPC position
    sf::Vector2f npcVelocity(0.f, 0.f); // Initial NPC velocity
    int npcCurrentFrame = 0;
    int npcAnimationRow = 1;

    // Animation setup
    sf::Clock playerAnimationClock;
    sf::Clock npcAnimationClock;
    sf::Clock movementClock;
    int playerCurrentFrame = 0;
    int playerAnimationRow = 1;

    // Generate the world map
    std::vector<std::vector<TileType>> world = generateWorld();

    // Create a view to control what part of the world is visible
    sf::View gameView(sf::FloatRect(0, 0, 800, 600)); // Initial view size
    sf::View hudView(sf::FloatRect(0, 0, 800, 600));  // HUD view for inventory

    // Setup inventory
    setupInventory();

    // Timer for NPC direction change
    sf::Clock npcMovementClock;
    const float npcMoveInterval = 2.0f; // Time in seconds between NPC direction changes

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Handle key press for player movement (WASD)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            velocity.y = -speed;
            playerAnimationRow = 2;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            velocity.y = speed;
            playerAnimationRow = 1;
        } else {
            velocity.y = 0.f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            velocity.x = -speed;
            playerAnimationRow = 3;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            velocity.x = speed;
            playerAnimationRow = 4;
        } else {
            velocity.x = 0.f;
        }

        // Update player position
        sf::Time deltaTime = movementClock.restart();
        playerPosition += velocity * deltaTime.asSeconds();

        // Player animation
        if (velocity.x == 0.f && velocity.y == 0.f) {
            playerCurrentFrame = 0;
        } else if (playerAnimationClock.getElapsedTime().asSeconds() > 0.15f) {
            playerCurrentFrame = (playerCurrentFrame + 1) % 4;
            playerAnimationClock.restart();
        }

        sf::IntRect playerFrame(playerCurrentFrame * TILE_SIZE, (playerAnimationRow - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        playerSprite.setTextureRect(playerFrame);

        // NPC movement logic: Move in a random direction at intervals
        if (npcMovementClock.getElapsedTime().asSeconds() > npcMoveInterval) {
            // Choose a random direction for the NPC
            int randomDirection = rand() % 4; // 0 = up, 1 = down, 2 = left, 3 = right
            float npcSpeed = 50.0f; // NPC speed

            if (randomDirection == 0 && npcPosition.y > 0) { // Move up
                npcVelocity = sf::Vector2f(0.f, -npcSpeed);
                npcAnimationRow = 2;
            } else if (randomDirection == 1 && npcPosition.y < MAP_HEIGHT * TILE_SIZE - TILE_SIZE) { // Move down
                npcVelocity = sf::Vector2f(0.f, npcSpeed);
                npcAnimationRow = 1;
            } else if (randomDirection == 2 && npcPosition.x > 0) { // Move left
                npcVelocity = sf::Vector2f(-npcSpeed, 0.f);
                npcAnimationRow = 3;
            } else if (randomDirection == 3 && npcPosition.x < MAP_WIDTH * TILE_SIZE - TILE_SIZE) { // Move right
                npcVelocity = sf::Vector2f(npcSpeed, 0.f);
                npcAnimationRow = 4;
            }

            npcMovementClock.restart(); // Reset the movement timer
        }

        npcPosition += npcVelocity * deltaTime.asSeconds();

        // NPC animation
        if (npcVelocity.x == 0.f && npcVelocity.y == 0.f) {
            npcCurrentFrame = 0;
        } else if (npcAnimationClock.getElapsedTime().asSeconds() > 0.15f) {
            npcCurrentFrame = (npcCurrentFrame + 1) % 4;
            npcAnimationClock.restart();
        }

        sf::IntRect npcFrame(npcCurrentFrame * TILE_SIZE, (npcAnimationRow - 1) * TILE_SIZE, TILE_SIZE, TILE_SIZE);
        npcSprite.setTextureRect(npcFrame);

        // Update game view
        gameView.setCenter(playerPosition);
        window.setView(gameView);

        // Render the world
        window.clear();
        renderWorld(window, world, grassSprite, waterSprite);

        // Render the player
        playerSprite.setPosition(playerPosition);
        window.draw(playerSprite);

        // Render the NPC
        npcSprite.setPosition(npcPosition);
        window.draw(npcSprite);

        // Render inventory
        drawInventory(window, hudView);

        window.display();
    }

    return 0;
}
