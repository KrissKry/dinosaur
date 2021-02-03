#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "sheep.hpp"

class GameProcessV2 {

    private:
        sf::RectangleShape rect;
        sf::RenderWindow window;
        Sheep sheep;
        sf::Clock clock;
    public:
        GameProcessV2() : window(sf::VideoMode(800, 600), "ecksdee")
        {
            // sf::RenderWindow window(sf::VideoMode(800, 600), "ecksdee");
            rect.setPosition(300, 200);
            rect.setFillColor(sf::Color(100, 200, 0, 100));
            clock.restart();
            sheep.scale(sf::Vector2f(10, 10));
            sheep.move(sf::Vector2f(100,100));
        }
        ~GameProcessV2(){}

        [[noreturn]] void run() {
            sf::Event event;
            sf::Time time;

            window.setVerticalSyncEnabled(true);
            
            while (true) {
                if (window.isOpen()) {
                    
                    while (window.pollEvent(event)) {
                        if ( event.type == sf::Event::Closed)
                            window.close();
                    }
                        


                    time = clock.getElapsedTime();
                    sheep.animate( time.asMilliseconds() );
                    clock.restart();

                    window.clear();

                    window.draw(rect);
                    sheep.draw(window);

                    window.display();
                    
                    // std::cout << "Re" << std::endl;
                }
            }
        }
};