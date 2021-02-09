#include <SFML/Graphics.hpp>

#include <mutex>
#include <thread>
#include <iostream>

#include "../randomgenerator.hpp"
#include "gameutil.hpp"

class Obstacle {

    private:

        const char JUMP_CHAR = 'w';
        const char DUCK_CHAR = 's';
        const sf::Vector2f JUMP_OBSTACLE = {GAME_WIDTH, OBSTACLE_JMP_Y};
        const sf::Vector2f DUCK_OBSTACLE = {GAME_WIDTH, OBSTACLE_DCK_Y};
        const sf::Vector2f SIZE = {10, 40};
        const std::string TEXT_BASE = "CURRENT MOVE: ";

        sf::RectangleShape rect;
        sf::Text obstacle_text;
        sf::Clock move_clock;

        
        char obstacle_key;      //'w', 's', or ' '
        float elapsed_time{};   //
        float deadline;         //time in millis to correctly dodge obstacle
        float cooldown{};       //period of time when obstacle should not be generated
        float cooldown_alpha;
        bool on_cooldown{};
        RandomGenerator rg;
        std::mutex mtx;

    public:
        Obstacle(sf::Font& font);

        void move(sf::Vector2f vector);

        void draw(sf::RenderWindow& window);

        void generateObstacle();

        void setCooldown();// { cooldown = 2000.0f; }

        void cleanup(float deadline);

        void animate(float delta_time); 

        char getCurrentKey() const { return obstacle_key; }

        sf::Vector2f getPosition() const { return rect.getPosition(); }

        float getElapsedTime() const { return elapsed_time; }

        float getCooldown() const { return cooldown; }

        int getSleepTime() { return rg.generateSleepTime(); }

                            //getrekt son
        sf::RectangleShape getRect() const { return rect; }

        bool isOnCooldown() const { return on_cooldown; }

        bool obstacleIsOver() const { return rect.getGlobalBounds().left <= 0; }
};