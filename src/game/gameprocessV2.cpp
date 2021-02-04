#include "../../include/game/gameprocessV2.hpp"


GameProcessV2::GameProcessV2() 
: window(sf::VideoMode(800, 600), "Mloda owca raper czlowieniu")
{
    
    window.clear();
    window.display();
    // window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(15);
    animate_clock.restart();
    move_clock.restart();

    sheep = std::make_unique<Sheep>(250.0f);
    sheep->scale(sf::Vector2f(10, 10));
    sheep->move(sf::Vector2f(200,100));


    std::string filename = "../assets/gamedev/cosmos_logic_demo.ttf";

    if ( !font.loadFromFile(filename)) {}
        // std::cout << "[ERR] Font failed to load .-." << std::endl;
    
    scoreboard = std::make_unique<Scoreboard>(font);

    obstacle = std::make_unique<Obstacle>(font, sf::Color(100,100,100,255));


    
    communication_thread = startCommunication();


}




GameProcessV2::~GameProcessV2() {

    sheep.reset();
    scoreboard.reset();
    obstacle.reset();

    if (communication_thread.joinable())
        communication_thread.join();
}


void GameProcessV2::communication() {

    while (!isOver) {
        awaitSignal();
        validateSignal();
    }
}


void GameProcessV2::awaitSignal() {
    std::cout << "awaiting signal" << std::endl << std::flush;
    in_queue.pop(&message_in);
}

void GameProcessV2::validateSignal() {
    std::cout << "validating signal" << std::endl << std::flush;
    //correct key received
    if (message_in.key == message_out.key && !isOver) {

        sf::Time temp = move_clock.getElapsedTime();
        move_clock.restart();


        //temporary - przeniesc gdzies gdzie bedzie aktualizowac na biezaco 
        if (temp.asMilliseconds() > obstacle_time) {
            isOver = true;
            std::cout << "[G A M E] Exceeded time. Game over." << std::endl << std::flush;
        }
        else {
            std::cout << "[G A M E] Success with validating" << std::endl << std::flush;
            scoreboard->addPoint();
            //to-do print SUCCESS text
            //delay generation for some time idk
            usleep(1000000);
            nextObstacle();
            sendRequest();
        }
        
    } else {
        std::cout << "[G A M E] its wong" << std::endl << std::flush;
        // sendRequest();
    }
}

void GameProcessV2::nextObstacle() {
    
    obstacle->cleanup();
    obstacle->generateObstacle();
    message_out.key = obstacle->getCurrentKey();
    // std::cout << "new obstacle" << std::endl << std::flush;
}

void GameProcessV2::sendRequest() {

    // std::cout << "trying sending request" << std::endl << std::flush;
    out_queue.push(&message_out);
    // std::cout << "post sending request " << std::endl << std::flush;
}



[[noreturn]] void GameProcessV2::run()
{
    sf::Event event;
    sf::Time delta_time;
    usleep(2500000);
    // std::cout << "[G A M E] Pre next obstable" << std::endl << std::flush;
    //obstacle->generateObstacle();
    nextObstacle();
    // std::cout << "[G A M E] pre send reqest" << std::endl << std::flush;

    sendRequest();
    // std::cout << "[G A M E] Loop pre" << std::endl << std::flush;
    
    int cnt = 0;
    while (true) {

        if (window.isOpen()) {
            
            while (window.pollEvent(event)) {
                if ( event.type == sf::Event::Closed)
                    window.close();
            }
                

            delta_time = animate_clock.getElapsedTime();
            sheep->animate( delta_time.asMilliseconds() );
            animate_clock.restart();

            // std::cout << "loop " << ++cnt << std::endl;


            window.clear();
            if (!isOver) {
                sheep->draw(window);
                scoreboard->draw(window);
                obstacle->draw(window);
            }
            else {
                // std::cout << "Over"
                scoreboard->draw(window);
            }
            window.display();

        }
    }
}
