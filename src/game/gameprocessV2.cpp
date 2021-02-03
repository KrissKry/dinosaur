#include "../../include/game/gameprocessV2.hpp"


GameProcessV2::GameProcessV2() 
: window(sf::VideoMode(800, 600), "Mloda owca raper czlowieniu")
{
    
    clock.restart();

    sheep = new Sheep(250.0f);
    sheep->scale(sf::Vector2f(10, 10));
    sheep->move(sf::Vector2f(200,100));



    std::string filename = "../assets/gamedev/cosmos_logic_demo.ttf";

    if ( !font.loadFromFile(filename)) {}
        // std::cout << "[ERR] Font failed to load .-." << std::endl;
    
    scoreboard = new Scoreboard(font);



    window.setVerticalSyncEnabled(true);
}



GameProcessV2::~GameProcessV2() {
    delete sheep;
}




[[noreturn]] void GameProcessV2::run()
{
    sf::Event event;
    sf::Time time;


    
    while (true) {

        if (window.isOpen()) {
            
            while (window.pollEvent(event)) {
                if ( event.type == sf::Event::Closed)
                    window.close();
            }
                

            time = clock.getElapsedTime();
            sheep->animate( time.asMilliseconds() );
            clock.restart();

            scoreboard->addPoint();

            window.clear();

            sheep->draw(window);
            scoreboard->draw(window);
            
            window.display();

        }
    }
}
