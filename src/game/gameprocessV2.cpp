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

    screenoverlay = new ScreenOverlay(OBSTACLE_COOLDOWN, font);
    
    
    
    animate_clock.restart();
    move_clock.restart();


    communication_thread = std::thread(&GameProcessV2::communication, this);
}


GameProcessV2::~GameProcessV2() {

    sheep.reset();
    scoreboard.reset();
    obstacle.reset();
    
    delete screenoverlay;

    if (communication_thread.joinable())
        communication_thread.join();
}


void GameProcessV2::communication() {

    while (!isOver) {
        awaitSignal();
        validateSignal();
    }
}

//wait for signal in message queue
void GameProcessV2::awaitSignal() {
    in_queue.pop(&message_in);
}

void GameProcessV2::validateSignal() {

    /*//correct key received
    if (message_in.key == message_out.key && !isOver) {

        sf::Time temp = move_clock.getElapsedTime();
        


        //exceeded time to dodge the obstacle
        if (temp.asMilliseconds() > obstacle_time) {
          
            endGame();
            screenoverlay->gameover();
        
        //success in dodging the obstacle
        } else {

            scoreboard->addPoint();
            screenoverlay->success();
            //to-do print SUCCESS text
            obstacle->setCooldown();
            usleep( obstacle->getCooldown()*1000 );
            
            nextObstacle();
            move_clock.restart();
            sendRequest();
        }
        

    //wrong key received
    } else {

        sf::Time temp = move_clock.getElapsedTime();

        //if there is still time left
        if (temp.asMilliseconds() < obstacle_time) {
            sendRequest();

        //wrong key and player ran out of time
        } else {
            endGame();
            screenoverlay->gameover();
        }
    }

*/
    //to-do nowy sposób walidacji ruchu - ruch i sprawdzenie kolizji?
    sheep->validateMovement(message_in.key);

    sf::Time temp = move_clock.getElapsedTime();

    if (temp.asMilliseconds() > obstacle_time) {

        if( sheep->getCollided() ) {

            endGame();
            screenoverlay->gameover();
        
        
        } else {
            
            continueGame();
            sendRequest();
        }


    } else {
        sendRequest();
    }


}

void GameProcessV2::continueGame() {
    
    scoreboard->addPoint();
    screenoverlay->success();
    
    //to-do print SUCCESS text
    obstacle->setCooldown();
    // usleep( obstacle->getCooldown() * 1000 );
    
    nextObstacle();
    move_clock.restart();
}

void GameProcessV2::nextObstacle() {
    
    obstacle_time *= GAME_SCALING_FACTOR;
    std::cout << "[GAME] New obstacle time: " << obstacle_time << std::endl;

    obstacle->cleanup(obstacle_time);

//to blokuje owce od skoku
    do {
        obstacle->generateObstacle();
        message_out.key = obstacle->getCurrentKey();

        if (message_out.key == ' ')
            usleep( obstacle->getSleepTime() );

    } while ( message_out.key == ' ' );
}


void GameProcessV2::sendRequest() {
    out_queue.push(&message_out);
}


void GameProcessV2::endGame() {
    std::cout << "[GAME] Exceeded time. Game over." << std::endl << std::flush;
    isOver = true;
}


[[noreturn]] void GameProcessV2::run()
{
    sf::Event event;
    sf::Time delta_time;

    usleep(150000);

    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Mloda owca raper czlowieniu");
    window.setFramerateLimit(60);

    nextObstacle();
    sendRequest();


    while (true) {

        if (window.isOpen()) {
            
            while (window.pollEvent(event)) {
                if ( event.type == sf::Event::Closed)
                    window.close();
            }
                
            /* Animation part */
            if (!isOver) {
                delta_time = animate_clock.getElapsedTime();
                sheep->setCollided ( collisionchecker.checkCollision(sheep->getBody(), obstacle->getRect()) );
                sheep->animate( delta_time.asMilliseconds() );
                sheep->updatePosition();
                obstacle->animate( delta_time.asMilliseconds() );
                screenoverlay->animate( delta_time.asMilliseconds() );
                animate_clock.restart();
            
            } else {
                delta_time = animate_clock.getElapsedTime();
                screenoverlay->animate( delta_time.asMilliseconds() );
                animate_clock.restart();
            }


            /* Render part */
            window.clear( sf::Color(250, 225, 150, 255));

            if (!isOver) {
                
                screenoverlay->draw(window);
                window.draw(floor);
                scoreboard->draw(window);
                sheep->draw(window);
                obstacle->draw(window);

            } else {
                //to-do game over overlay
                screenoverlay->draw(window);
                scoreboard->draw(window);
            }

            window.display();

        }
    }
}
