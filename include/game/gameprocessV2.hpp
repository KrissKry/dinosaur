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

        sf::RenderWindow window;
        sf::Clock animate_clock;
        sf::Clock move_clock;
        sf::Font font;
        sf::Time generate_time;

        unsigned int obstacle_time = 50000.0f; //5000ms
        //to-do reduce time;
        //to-do animate object movement
        //to-do sheep jump animation or duck
        //to-do etc (...) xdd
        bool isOver = false;


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


        std::thread startCommunication() { 
            std::cout << "THREAD BITCHHH\n";
            return std::thread(&GameProcessV2::communication, this); }

    public:

        GameProcessV2();
        ~GameProcessV2();


        [[noreturn]] void run();
};