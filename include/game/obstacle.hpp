#include <SFML/Graphics.hpp>

#include <mutex>
#include <iostream>

#include "../randomgenerator.hpp"

class Obstacle {

    private:
        // sf::RectangleShape rectangle;
        //sf::Texture texture;
        const sf::Vector2f JUMP_OBSTACLE = {500, 100};
        const sf::Vector2f DUCK_OBSTACLE = {500, 500};
        const sf::Vector2f SIZE = {50, 50};
        
        // sf::Sprite sprite;
        sf::RectangleShape rect;
        sf::Text obstacle_text;
        RandomGenerator rg;
        char obstacle_key;


        std::mutex mtx;

    public:
        Obstacle(sf::Font& font, sf::Color color);

        void move(sf::Vector2f vector);

        void draw(sf::RenderWindow& window);

        void generateObstacle();

        void cleanup();

        void animate(); //to-do movement

        char getCurrentKey() { 
        
            std::cout << "[OBSTACLE] Trying to return key XD" << std::endl << std::flush;
        
            return obstacle_key;     
        }
};