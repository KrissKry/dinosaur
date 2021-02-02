#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class GameProcessV2 {

    private:
        sf::RectangleShape rect;
        sf::RenderWindow window;
    public:
        GameProcessV2() : window(sf::VideoMode(800, 600), "ecksdee")
        {
            // sf::RenderWindow window(sf::VideoMode(800, 600), "ecksdee");
            rect.setPosition(300, 200);
            rect.setFillColor(sf::Color(100, 200, 0, 255));
        }
        ~GameProcessV2(){}

        [[noreturn]] void run() {
            sf::Event event;

            window.setVerticalSyncEnabled(true);
            
            while (true) {
                if (window.isOpen()) {
                    
                    while (window.pollEvent(event)) {
                        if ( event.type == sf::Event::Closed)
                            window.close();
                    }
                        
                    window.clear();

                    window.draw(rect);

                    window.display();
                    
                    // std::cout << "Re" << std::endl;
                }
            }
        }
};