// #include "../include/game/dinosaur.hpp"
#include "../../include/game/sheep.hpp"


Sheep::Sheep(float frame_time) {
    
    texture.loadFromFile(sheep_sprite);


    animation = new Animation(&texture, 2, frame_time);

    body.setTexture(texture);
    body.setTextureRect( animation->getCurrentTexture() );
}

Sheep::~Sheep() {
    delete animation;
}

void Sheep::move(sf::Vector2f vec) {
    
    //absolute position
    body.setPosition(vec);
}

void Sheep::scale(sf::Vector2f vec) {
    body.setScale(vec);
}

void Sheep::rotate(float degrees) {
    body.rotate(degrees);
}

void Sheep::draw(sf::RenderWindow& window) {
    window.draw(body);
}
void Sheep::animate(float delta_time) {

    animation->update(delta_time);

    body.setTextureRect( animation->getCurrentTexture() );
}