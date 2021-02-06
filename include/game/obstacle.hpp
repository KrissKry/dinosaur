#include <SFML/Graphics.hpp>

#include <mutex>
#include <iostream>

#include "../randomgenerator.hpp"
#include "gameutil.hpp"

class Obstacle {

    private:


        const sf::Vector2f JUMP_OBSTACLE = {GAME_WIDTH - 40, OBSTACLE_JMP_Y};
        const sf::Vector2f DUCK_OBSTACLE = {GAME_WIDTH - 40, OBSTACLE_DCK_Y};
        const sf::Vector2f SIZE = {40, 40};
        
        // sf::Sprite sprite;
        sf::RectangleShape rect;
        sf::Text obstacle_text;
        sf::Text cooldown_text;

        sf::Clock move_clock;
        RandomGenerator rg;
        
        char obstacle_key;      //
        float elapsed_time{};   //
        float deadline;         //time in millis to correctly dodge obstacle?
        float cooldown{};       //period of time when obstacle should not be generated

        std::mutex mtx;

    public:
        Obstacle(sf::Font& font);

        void move(sf::Vector2f vector);

        void draw(sf::RenderWindow& window);

        void generateObstacle();

        void setCooldown() { cooldown = 2000.0f; }

        void cleanup(float deadline);

        void animate(float delta_time); //to-do movement

        char getCurrentKey() const { return obstacle_key; }

        sf::Vector2f getPosition() const { return rect.getPosition(); }

        float getElapsedTime() const { return elapsed_time; }

        float getCooldown() const { return cooldown; }

};