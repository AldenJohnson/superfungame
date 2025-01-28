#include <SFML/Graphics.hpp>
#include "NPC.hpp"
#include "Player.hpp" // Include the new Player class
#include "Items.hpp"
#include "World.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Player and NPC Animations", sf::Style::Resize);

    // Load textures
    sf::Texture grassTexture, waterTexture, playerTexture, npcTexture, footballTexture;
    if (!grassTexture.loadFromFile("images/grass.png") ||
        !waterTexture.loadFromFile("images/water.png") ||
        !playerTexture.loadFromFile("images/player_spritesheet.png") ||
        !npcTexture.loadFromFile("images/npc_spritesheet.png") ||
        !footballTexture.loadFromFile("images/football.png")) {
        return -1;
    }

    // Create sprites
    sf::Sprite grassSprite(grassTexture), waterSprite(waterTexture);
    sf::Sprite npcSprite(npcTexture);
    npcSprite.setOrigin(64, 64);  // Assuming npc sprite is 128x128

    // Create NPC, Player, and Football objects
    NPC npc(npcTexture, 50.0f);
    Player player(playerTexture, 200.0f); // Create player with specific speed

    // World generation
    auto world = generateWorld();

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

        npc.update(deltaTime);

        // Render the world
        window.clear();
        renderWorld(window, world, grassSprite, waterSprite);

        // Render NPC, player
        npc.render(window);
        player.render(window);

        window.display();
    }

    return 0;
}
