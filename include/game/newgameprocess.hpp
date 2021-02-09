#include "sheep.hpp"
#include "scoreboard.hpp"
#include "obstacle.hpp"
#include "../messagequeue.hpp"
#include "gameutil.hpp"
#include "screenoverlay.hpp"
#include "collisionchecker.hpp"

class NewGameProcess {


    private:

        Sheep* sheep;
        Scoreboard* scoreboard;
        Obstacle* obstacle;
        ScreenOverlay* screenoverlay;
        CollisionChecker collisionchecker;

        sf::Clock animation_clock;
        sf::Clock movement_clock;
        sf::Font font;
        sf::RectangleShape floor;

        const std::string font_file = "../assets/gamedev/shakerato.otf";

        std::thread communication_thread;
        std::thread validation_thread;

        bool is_over = false;
        float obstacle_time = 2500.0f;


        game_message message_out; 
        game_message message_in;
        
        MessageQueue out_queue = {true, MQ_PRODUCER_GAME};
        MessageQueue in_queue = {false, MQ_CLIENT_GAME};

        std::mutex msg_in_acc;
        std::mutex msg_out_acc;

        void communication();
        void awaitSignal();
        void sendSignal();


        void validation();
        void nextObstacle();

        void continueGame();
        void endGame();
    public:

        NewGameProcess();
        ~NewGameProcess();

        [[noreturn]] void run();

};