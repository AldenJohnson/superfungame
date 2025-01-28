#include "Items.hpp"
#include <cmath>

Arrow::Arrow(float radius) : speed(500.0f) { // Default speed of the arrow
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Red); // Set arrow color to red
    shape.setOrigin(radius, radius);   // Center the origin of the arrow
    shape.setScale(2.f, 2.f); // Scale arrows (adjust as needed)
}

void Arrow::update(float deltaTime) {
    // Update the arrow's position based on its velocity
    shape.move(velocity * speed * deltaTime);
}