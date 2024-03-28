#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>

// Define constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PLAYER_SPEED = 5.0f;
const float BULLET_SPEED = 8.0f;
const int ENEMY_COUNT = 10;

// Player class
class Player {
public:
    sf::RectangleShape shape;

    Player() {
        shape.setSize(sf::Vector2f(50.0f, 50.0f));
        shape.setFillColor(sf::Color::Green);
        shape.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 100);
    }

    void move(float offsetX, float offsetY) {
        shape.move(offsetX * PLAYER_SPEED, offsetY * PLAYER_SPEED);
    }
};

// Bullet class
class Bullet {
public:
    sf::RectangleShape shape;
    bool active;

    Bullet() {
        shape.setSize(sf::Vector2f(5.0f, 20.0f));
        shape.setFillColor(sf::Color::Red);
        active = false;
    }

    void move() {
        shape.move(0, -BULLET_SPEED);
    }
};

// Enemy class
class Enemy {
public:
    sf::RectangleShape shape;

    Enemy(float x, float y) {
        shape.setSize(sf::Vector2f(50.0f, 50.0f));
        shape.setFillColor(sf::Color::Red);
        shape.setPosition(x, y);
    }

    void move() {
        shape.move(0, 1.0f);
    }
};

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Space Shooter Game");

    // Create player
    Player player;

    // Create bullets
    std::vector<Bullet> bullets(10);

    // Create enemies
    std::vector<Enemy> enemies;
    for (int i = 0; i < ENEMY_COUNT; ++i) {
        enemies.push_back(Enemy(rand() % WINDOW_WIDTH, -50.0f * i));
    }

    // Main game loop
    while (window.isOpen()) {
        // Event handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Player movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            player.move(-1.0f, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            player.move(1.0f, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            for (auto& bullet : bullets) {
                if (!bullet.active) {
                    bullet.active = true;
                    bullet.shape.setPosition(player.shape.getPosition().x + player.shape.getSize().x / 2 - bullet.shape.getSize().x / 2,
                                             player.shape.getPosition().y);
                    break;
                }
            }
        }

        // Bullets movement
        for (auto& bullet : bullets) {
            if (bullet.active) {
                bullet.move();
                if (bullet.shape.getPosition().y < 0)
                    bullet.active = false;
            }
        }

        // Enemies movement
        for (auto& enemy : enemies) {
            enemy.move();
        }

        // Collision detection
        for (auto& bullet : bullets) {
            if (bullet.active) {
                for (auto& enemy : enemies) {
                    if (bullet.shape.getGlobalBounds().intersects(enemy.shape.getGlobalBounds())) {
                        bullet.active = false;
                        enemy.shape.setPosition(rand() % WINDOW_WIDTH, -50.0f);
                    }
                }
            }
        }

        // Clear window
        window.clear();

        // Draw objects
        window.draw(player.shape);
        for (auto& bullet : bullets) {
            if (bullet.active)
                window.draw(bullet.shape);
        }
        for (auto& enemy : enemies) {
            window.draw(enemy.shape);
        }

        // Display window
        window.display();
    }

    return 0;
}
