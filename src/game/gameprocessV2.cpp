#include "../../include/game/gameprocessV2.hpp"


GameProcessV2::GameProcessV2() {
    

    if ( !font.loadFromFile(font_file)) {
        std::cout << "[ERR] Font failed to load .-." << std::endl;
    }

    sheep = std::make_unique<Sheep>(250.0f);
    sheep->scale(sf::Vector2f(4.5f, 4.5f));
    sheep->setPosition(sf::Vector2f(SHEEP_XPOS, SHEEP_YPOS));

    scoreboard = std::make_unique<Scoreboard>(font);

    obstacle = std::make_unique<Obstacle>(font);

    floor.setSize( sf::Vector2f(GAME_WIDTH, 0.25f * GAME_HEIGHT) );
    floor.setPosition( sf::Vector2f(0.0f, FLOOR_YPOS) );
    floor.setFillColor( sf::Color(220, 255, 210, 255) );

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

            obstacle->setCooldown();
            usleep( obstacle->getCooldown()*1000 );
            nextObstacle();
            sendRequest();
        }
        
    } else {

        sf::Time temp = move_clock.getElapsedTime();
        if (temp.asMilliseconds() < obstacle_time) {
            sendRequest();
        } else {
            std::cout << "[G A M E] its wong" << std::endl << std::flush;
            isOver = true;
        // sendRequest();
        }
    }
}



void GameProcessV2::nextObstacle() {
    
   
    obstacle->cleanup(obstacle_time);
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
    usleep(150000);
    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Mloda owca raper czlowieniu");
    
    window.setFramerateLimit(30);
    
    nextObstacle();
    sendRequest();
    // std::cout << "[GAME] b4 loop\n";
    while (true) {

        if (window.isOpen()) {
            
            while (window.pollEvent(event)) {
                if ( event.type == sf::Event::Closed)
                    window.close();
            }
                

            delta_time = animate_clock.getElapsedTime();
            sheep->animate( delta_time.asMilliseconds() );
            obstacle->animate( delta_time.asMilliseconds() );
            animate_clock.restart();
            // std::cout << "[GAME] loop\n";


            window.clear( sf::Color(250, 225, 150, 255));

            if (!isOver) {
                
                window.draw(floor);
                scoreboard->draw(window);
                sheep->draw(window);
                obstacle->draw(window);

            } else {

                scoreboard->draw(window);
            }

            window.display();

        }
    }
}
