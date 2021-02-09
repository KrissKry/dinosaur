#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <thread>

#include "sheep.hpp"
#include "scoreboard.hpp"
#include "obstacle.hpp"
#include "../messagequeue.hpp"
#include "gameutil.hpp"
#include "screenoverlay.hpp"
#include "collisionchecker.hpp"

class GameProcessV2 {

    private:

        
        sf::Clock animate_clock;
        sf::Clock move_clock;
        sf::Font font;
        sf::Time generate_time;
        sf::RectangleShape floor;

        const std::string font_file = "../assets/gamedev/shakerato.otf";




        float obstacle_time = 5000.0f; //10000ms //to-do change in the future
        bool isOver = false;
        bool firstRun = true;
        //to-do reduce time;
        //to-do animate object movement
        //to-do sheep jump animation or duck
        //to-do etc (...) xdd

        std::thread communication_thread;


        std::unique_ptr<Sheep> sheep;
        std::unique_ptr<Scoreboard> scoreboard;
        std::unique_ptr<Obstacle> obstacle;
        ScreenOverlay* screenoverlay;
        CollisionChecker collisionchecker;

        
        game_message message_out; 
        game_message message_in;
        
        MessageQueue out_queue = {true, MQ_PRODUCER_GAME};
        MessageQueue in_queue = {false, MQ_CLIENT_GAME};


        //connects awaitsignal and validatesignal, uses thread  to fucking work jesus
        void communication();
        void awaitSignal();
        void validateSignal();
        
        void nextObstacle();
        void sendRequest();

        void checkCollision( sf::Sprite sheep, sf::RectangleShape obstacle);
        
        void continueGame();
            //avengers
        void endGame();

    public:

        GameProcessV2();
        ~GameProcessV2();


        [[noreturn]] void run();
};