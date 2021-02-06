#include <SFML/Graphics.hpp>

#include <mutex>
#include <iostream>

#include "../randomgenerator.hpp"

class Obstacle {

    private:
        // sf::RectangleShape rectangle;
        //sf::Texture texture;
        const sf::Vector2f JUMP_OBSTACLE = {500, 250};
        const sf::Vector2f DUCK_OBSTACLE = {500, 400};
        const sf::Vector2f SIZE = {40, 40};
        
        // sf::Sprite sprite;
        sf::RectangleShape rect;
        sf::Text obstacle_text;
        RandomGenerator rg;
        char obstacle_key;


        std::mutex mtx;

    public:
        Obstacle(sf::Font& font);

        void move(sf::Vector2f vector);

        void draw(sf::RenderWindow& window);

        void generateObstacle();

        void cleanup();

        void animate(); //to-do movement

        char getCurrentKey() const { return obstacle_key; }
};