#include <SFML/Graphics.hpp>

class Obstacle {

    private:
        sf::RectangleShape rectangle;
        //sf::Texture texture;

    public:
        Obstacle(sf::Vector2f size, sf::Color color);

        void move(sf::Vector2f vec2);

        void draw(sf::Window& window);
}