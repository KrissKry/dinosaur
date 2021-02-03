#include <SFML/Graphics.hpp>

#include "animation.hpp"

class Sheep {

    private:
        Animation* animation;
        sf::Sprite body;
        sf::Texture texture;

    public:
        Sheep(float frame_time);
        ~Sheep();


        void draw(sf::RenderWindow& window);

        void animate(float delta_time);

        void scale(sf::Vector2f vec2);
        void move(sf::Vector2f vec2);
        void rotate(float degrees);
};