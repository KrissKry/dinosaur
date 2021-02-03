#include <SFML/Graphics.hpp>

class Animation {
    public:
        Animation(sf::Texture* texture, int image_count, float frame_time);
        ~Animation() {}

        // void draw(float delta_time);
        void update(float delta_time);
        
        sf::IntRect getCurrentTexture() const { return txtr_rect; }

    private:
        float frame_time;
        float elapsed_time;
        // sf::Texture texture;
        unsigned int image_count;
        sf::Vector2u current_image;
        sf::IntRect txtr_rect;
        
};
