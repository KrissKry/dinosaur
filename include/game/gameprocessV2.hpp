#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
// #include <iostre
#include "sheep.hpp"
#include "scoreboard.hpp"

class GameProcessV2 {

    private:

        sf::RenderWindow window;
        sf::Clock clock;
        sf::Font font;

        Sheep* sheep;
        Scoreboard* scoreboard;

    public:

        GameProcessV2();
        ~GameProcessV2();


        [[noreturn]] void run();
};