#include <SFML/Graphics.hpp>
#include "NPC.hpp"
#include "Player.hpp" // Include the new Player class
#include "Items.hpp"
#include "World.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Player and NPC Animations", sf::Style::Resize);

    // Load textures
    sf::Texture grassTexture, waterTexture, playerTexture, npcTexture, arrowTexture;
    if (!grassTexture.loadFromFile("images/grass.png") ||
        !waterTexture.loadFromFile("images/water.png") ||
        !playerTexture.loadFromFile("images/player_spritesheet.png") ||
        !npcTexture.loadFromFile("images/npc_spritesheet.png") ||
        !arrowTexture.loadFromFile("images/arrow.png")) {
        return -1;
    }

    // Create sprites
    sf::Sprite grassSprite(grassTexture), waterSprite(waterTexture);
    sf::Sprite npcSprite(npcTexture);
    npcSprite.setOrigin(64, 64);  // Assuming npc sprite is 128x128

    // Create NPC, Player, and container for Arrows
    NPC npc(npcTexture, 50.0f);
    Player player(playerTexture, 200.0f);
    std::vector<Arrow> arrows; // Store active arrows

    // World generation
    auto world = generateWorld();

    // Views for game and HUD
    sf::View gameView(sf::FloatRect(0, 0, 800, 600));
    sf::View hudView(sf::FloatRect(0, 0, 800, 600));

    sf::Clock clock; // Global clock for deltaTime

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Get deltaTime
        sf::Time deltaTime = clock.restart();

        // Handle player input (movement and animation control)
        player.handleInput();  // Handle player input in the Player class
        player.update(deltaTime); // Update player state (movement, animation)

        // Update NPC
        npc.update(deltaTime);

        // Shooting logic (space key)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            static sf::Clock shootClock; // Controls shooting rate
            if (shootClock.getElapsedTime().asSeconds() > 0.2f) { // Fire every 0.2 seconds
                Arrow arrow; // Create a new Arrow
                arrow.shape.setTexture(&arrowTexture); // Set the arrow texture
                arrow.shape.setRadius(5.0f);          // Set arrow size
                arrow.shape.setOrigin(5.f, 5.f);      // Center the origin

                arrow.shape.setPosition(player.getPosition()); // Start at player's position

                // Set arrow velocity based on player direction
        if (player.getAnimationRow() == 0) {       // Down
            arrow.velocity = sf::Vector2f(0.f, 1.f);
            arrow.shape.setRotation(180.f);        // Rotate arrow downward
        } else if (player.getAnimationRow() == 1) {  // Up
            arrow.velocity = sf::Vector2f(0.f, -1.f);
            arrow.shape.setRotation(0.f);
        } else if (player.getAnimationRow() == 2) {  // Left
            arrow.velocity = sf::Vector2f(-1.f, 0.f);
            arrow.shape.setRotation(270.f);       // Rotate arrow to the left
        } else if (player.getAnimationRow() == 3) {  // Right
            arrow.velocity = sf::Vector2f(1.f, 0.f);
            arrow.shape.setRotation(90.f);         // No rotation needed for right
        }

                arrows.push_back(arrow); // Add the arrow to the vector
                shootClock.restart(); // Reset shooting timer
            }
        }

        // Update arrows
        for (size_t i = 0; i < arrows.size(); ++i) {
            arrows[i].update(deltaTime.asSeconds());

            // Remove arrows that go off-screen
            if (arrows[i].shape.getPosition().x < 0 || arrows[i].shape.getPosition().x > 800 ||
                arrows[i].shape.getPosition().y < 0 || arrows[i].shape.getPosition().y > 600) {
                arrows.erase(arrows.begin() + i);
                --i; // Adjust index after removal
            }
        }

        // Update game view to follow the player
        gameView.setCenter(player.getPosition());
        window.setView(gameView);

        // Render the world
        window.clear();
        renderWorld(window, world, grassSprite, waterSprite);

        // Render NPC and player
        npc.render(window);
        player.render(window);

        // Render arrows
        for (const auto& arrow : arrows) {
            window.draw(arrow.shape);
        }

        window.display();
    }

    return 0;
}