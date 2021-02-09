#include <SFML/Graphics.hpp>
#include "gameutil.hpp"
#include <cmath>
#include <iostream>

class ScreenOverlay {

    private:
        float elapsed_time;
        float animation_time;
        // unsigned int current_alpha;
        int current_alpha;
        bool is_drawn;
        bool is_over = false;

        const unsigned int OVERLAY_INIT_ALPHA = 200;
        const unsigned int success_r = 165;
        const unsigned int success_g = 255;
        const unsigned int success_b = 140;

        const unsigned int fail_r_min = 127;
        const unsigned int fail_g_min = 0;
        const unsigned int fail_b_min = 0;
        const unsigned int fail_r_max = 230;
        const unsigned int fail_g_max = 133;
        const unsigned int fail_b_max = 133;
        const float PI = 3.14159f;

        sf::RectangleShape rect;
        sf::Text success_text;
        sf::Text failure_text;
    public:
        ScreenOverlay(float animation_time, sf::Font& font);
        // ~SuccessOverlay();

        void animate(float delta_time);

        void success();
        
        void gameover();

        void draw(sf::RenderWindow& window);
};