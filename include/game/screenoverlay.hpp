#include <SFML/Graphics.hpp>
#include "gameutil.hpp"

class ScreenOverlay {

    private:
        float elapsed_time;
        float animation_time;
        // unsigned int current_alpha;
        int current_alpha;
        bool is_drawn;

        const unsigned int OVERLAY_INIT_ALPHA = 200;
        const unsigned int success_r = 165;
        const unsigned int success_g = 255;
        const unsigned int success_b = 140;

        sf::RectangleShape rect;
    public:
        ScreenOverlay(float animation_time);
        // ~SuccessOverlay();

        void animate(float delta_time);

        void success();
        // void 

        void draw(sf::RenderWindow& window);
};