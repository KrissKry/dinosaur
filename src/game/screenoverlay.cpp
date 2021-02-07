#include "../../include/game/screenoverlay.hpp"


ScreenOverlay::ScreenOverlay(float animation_time) {

    this->animation_time = animation_time;
    elapsed_time = 0.0f;
    is_drawn = false;

    rect.setSize( sf::Vector2f(GAME_WIDTH, GAME_HEIGHT * 0.75f) );
    rect.setPosition( sf::Vector2f(0.0f,0.0f) );
    rect.setFillColor( sf::Color(success_r, success_g, success_b, OVERLAY_INIT_ALPHA) );
}

void ScreenOverlay::animate(float delta_time) {

    elapsed_time += delta_time;
    current_alpha = OVERLAY_INIT_ALPHA * (1 - elapsed_time / animation_time );

    if (current_alpha < 0) {
        current_alpha = 0;
        is_drawn = false;
    }

    rect.setFillColor ( sf::Color(success_r, success_g, success_b, current_alpha) );
}

void ScreenOverlay::draw(sf::RenderWindow& window) {

    if (is_drawn)
        window.draw(rect);
}

void ScreenOverlay::success() {
    elapsed_time = 0;
    is_drawn = true;
}
// void ScreenOverlay::