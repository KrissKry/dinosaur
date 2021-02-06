#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <thread>

#include "sheep.hpp"
#include "scoreboard.hpp"
#include "obstacle.hpp"
#include "../messagequeue.hpp"

// #define LOG 1

class GameProcessV2 {

    private:

        
        sf::Clock animate_clock;
        sf::Clock move_clock;
        sf::Font font;
        sf::Time generate_time;

        const std::string font_file = "../assets/gamedev/shakerato.otf";



        unsigned int obstacle_time = 50000.0f; //50000ms //to-do change in the future
        bool isOver = false;

        //to-do reduce time;
        //to-do animate object movement
        //to-do sheep jump animation or duck
        //to-do etc (...) xdd
        

        std::thread communication_thread;


        std::unique_ptr<Sheep> sheep;
        std::unique_ptr<Scoreboard> scoreboard;
        std::unique_ptr<Obstacle> obstacle;


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

    public:

        GameProcessV2();
        ~GameProcessV2();


        [[noreturn]] void run();
};