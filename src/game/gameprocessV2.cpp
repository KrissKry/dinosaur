#include "../../include/game/gameprocessV2.hpp"


GameProcessV2::GameProcessV2() {
    

    if ( !font.loadFromFile(font_file)) {
        std::cout << "[ERR] Font failed to load .-." << std::endl;
    }

    sheep = std::make_unique<Sheep>(250.0f);
    sheep->scale(sf::Vector2f(4.5f, 4.5f));
    sheep->move(sf::Vector2f(50, 300));

    scoreboard = std::make_unique<Scoreboard>(font);

    obstacle = std::make_unique<Obstacle>(font);

    animate_clock.restart();
    move_clock.restart();


    communication_thread = std::thread(&GameProcessV2::communication, this);
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
    in_queue.pop(&message_in);
}

void GameProcessV2::validateSignal() {

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

            scoreboard->addPoint();
            //to-do print SUCCESS text
            //delay generation for some time idk
            usleep(1500000);
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
}


void GameProcessV2::sendRequest() {

    out_queue.push(&message_out);
}



[[noreturn]] void GameProcessV2::run()
{
    sf::Event event;
    sf::Time delta_time;

    // usleep(2000000);
    
;
    
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800, 600), "Mloda owca raper czlowieniu");
    
    window.setFramerateLimit(30);
    usleep(150000);
    nextObstacle();
    sendRequest();

    while (true) {

        if (window.isOpen()) {
            
            while (window.pollEvent(event)) {
                if ( event.type == sf::Event::Closed)
                    window.close();
            }
                

            delta_time = animate_clock.getElapsedTime();
            sheep->animate( delta_time.asMilliseconds() );
            animate_clock.restart();



            window.clear( sf::Color(250, 225, 150, 255));

            if (!isOver) {
                sheep->draw(window);
                scoreboard->draw(window);
                obstacle->draw(window);
            
            } else {

                scoreboard->draw(window);
            }

            window.display();

        }
    }
}
