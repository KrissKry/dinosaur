#include "../../include/game/newgameprocess.hpp"


NewGameProcess::NewGameProcess() {
    
    
    if ( !font.loadFromFile(font_file)) {
        std::cout << "[ERR] Font failed to load .-." << std::endl;
    }

    // sheep = std::make_unique<Sheep>(250.0f);
    sheep = new Sheep(250.0f);
    sheep->scale(sf::Vector2f(4.5f, 4.5f));
    sheep->setPosition(sf::Vector2f(0, SHEEP_YPOS));

    // scoreboard = std::make_unique<Scoreboard>(font);
    scoreboard = new Scoreboard(font);
    obstacle = new Obstacle(font);
    
    
    // obstacle->generateObstacle();

    // obstacle = std::make_unique<Obstacle>(font);

    floor.setSize( sf::Vector2f(GAME_WIDTH, 0.25f * GAME_HEIGHT) );
    floor.setPosition( sf::Vector2f(0.0f, FLOOR_YPOS) );
    floor.setFillColor( sf::Color(220, 255, 210, 255) );

    screenoverlay = new ScreenOverlay(OBSTACLE_COOLDOWN, font);
    
    
    
    animation_clock.restart();
    movement_clock.restart();

    nextObstacle();

    //jeden watek odpowiada za nadawanie i odbieranie komunikatow TYLKO
    communication_thread = std::thread(&NewGameProcess::communication, this);
    //drugi watek za sprawdzenie komunikatu i odpowiednia reakcje
    validation_thread = std::thread(&NewGameProcess::validation, this);

    //glowny watek za ciagle rysowanie tego gowna
}

NewGameProcess::~NewGameProcess() {
    delete sheep;
    delete scoreboard;
    delete obstacle;
    delete screenoverlay;


}

void NewGameProcess::communication() {

    while(!is_over) {
        sendSignal();

        awaitSignal();
        std::this_thread::sleep_for( std::chrono::milliseconds(5) );
        // sendSignal();
    }
}

void NewGameProcess::awaitSignal() {
    in_queue.pop(&message_in);
}

void NewGameProcess::sendSignal() {
    out_queue.push(&message_out);
}


void NewGameProcess::validation() {

    while(!is_over) {

        sheep->validateMovement(message_in.key);
        // sf::Time temp = movement_clock.getElapsedTime();

        if ( obstacle->obstacleIsOver() || sheep->getCollided() ) {
            

            if( sheep->getCollided() ) {
                endGame();
                screenoverlay->gameover();
            
            } else {
            continueGame();
            }
        }
        
        std::this_thread::sleep_for( std::chrono::milliseconds(10) );
    }
}

void NewGameProcess::endGame() {
    std::cout << "[GAME] Over.\n" << std::flush;

    is_over = true;
}

void NewGameProcess::continueGame() {


    scoreboard->addPoint();
    screenoverlay->success();
    obstacle->setCooldown();


    obstacle_time *= GAME_SCALING_FACTOR;
    std::cout << "[GAME] New obstacle time: " << obstacle_time << "\n" << std::flush;
    obstacle->cleanup(obstacle_time);


    nextObstacle();
    // obstacle->cleanup();
    // obstacle->setObstacleTime(obstacle_time); //to-do
}

void NewGameProcess::nextObstacle() {
    do {
        obstacle->generateObstacle();
        message_out.key = obstacle->getCurrentKey();

        if (message_out.key == ' ')
            usleep( obstacle->getSleepTime() );
            // std::this_thread::sleep_for(std::chrono::milliseconds( ))
    } while ( message_out.key == ' ');
}

[[noreturn]] void NewGameProcess::run() {

    sf::Event event;
    sf::Time delta_time;

    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), "Mloda owca raper czlowieniu");
    window.setFramerateLimit(60);

    bool collision;

    while (true) {

        if (window.isOpen()) {
            

            while (window.pollEvent(event)) {
                if ( event.type == sf::Event::Closed)
                    window.close();
            }

            /* animation part */
            if (!is_over) {
                delta_time = animation_clock.getElapsedTime();

                collision = collisionchecker.checkCollision( sheep->getBody(), obstacle->getRect() );

                sheep->setCollided(collision);

                sheep->animate( delta_time.asMilliseconds());
                sheep->updateMovement();

                obstacle->animate( delta_time.asMilliseconds() );
                screenoverlay->animate(delta_time.asMilliseconds() );

                animation_clock.restart();

                
            } else {
                delta_time = animation_clock.getElapsedTime();
                screenoverlay->animate( delta_time.asMilliseconds() );
                animation_clock.restart();
            }




            /* render part */
            window.clear( sf::Color(250, 225, 150, 255));


            if (!is_over) {
                screenoverlay->draw(window);
                window.draw(floor);
                scoreboard->draw(window);
                sheep->draw(window);
                obstacle->draw(window);

            } else {

                screenoverlay->draw(window);
                scoreboard->draw(window);
            }

            window.display();
        }
    }
}