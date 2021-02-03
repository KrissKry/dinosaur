#include "../../include/game/animation.hpp"


Animation::Animation(sf::Texture* texture, int image_count, float frame_time) {


    this->image_count = image_count - 1; //if image_count == 2, then we index with 0 and 1
    this->frame_time = frame_time;
    this->elapsed_time = 0.0f;
    
    current_image.x = 0;
    current_image.y = 0;

    txtr_rect.height = texture->getSize().y;
    txtr_rect.width = texture->getSize().x / (float)image_count;

}

void Animation::update(float delta_time) {

    elapsed_time += delta_time;

    if (elapsed_time > frame_time) {
        
        
        ++current_image.x;

        if (current_image.x > image_count)
            current_image.x = 0;


        
        
        txtr_rect.left = current_image.x * txtr_rect.width;
        


        elapsed_time -= frame_time;
    }
}