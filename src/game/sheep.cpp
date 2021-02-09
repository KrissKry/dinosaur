// #include "../include/game/dinosaur.hpp"
#include "../../include/game/sheep.hpp"


Sheep::Sheep(float frame_time) {
    
    if ( !texture.loadFromFile(sheep_sprite) ) {}
        // std::cout << "[SHEEP] Couldn't load texture" << std::endl;


    animation = new Animation(&texture, 2, frame_time);

    body.setTexture(texture);
    body.setTextureRect( animation->getCurrentTexture() );
}

Sheep::~Sheep() {
    delete animation;
}

void Sheep::animate(float delta_time) {

    animation->update(delta_time);

    body.setTextureRect( animation->getCurrentTexture() );
}


void Sheep::validateMovement(char key) {

    if (!is_jumping && key == 'w') {
        
        is_jumping = true;
        is_ducking = false;
        velocity_y -= acceleration_y;
    

    } else if (!is_jumping && key == 's') {

        is_ducking = true;
        is_jumping = false;


    } else {
        is_ducking = false;
    }
}


void Sheep::updateMovement() {

    if (is_jumping) {
        //still jumping
        velocity_y += GAME_GRAVITY;
        body.move( sf::Vector2f(0.0f, (float)velocity_y) );

        //finished jump, reset Y position
        if (body.getPosition().y > SHEEP_YPOS) {
            body.setPosition( sf::Vector2f( 0.0f, SHEEP_YPOS) );
            is_jumping = false;
            velocity_y = 0;
        }

    } else if (is_ducking) {
        
        //move body down - temporary solution -> will make a sprite in future
        body.setPosition( sf::Vector2f( 0.0f, SHEEP_YPOS + 50));

    } else {
        //reset body position
        body.setPosition( sf::Vector2f( 0.0f, SHEEP_YPOS));
    }
}

void Sheep::setCollided(bool collided) {

    //set collision only once
    if (this->collided != true)
        this->collided = collided;
}