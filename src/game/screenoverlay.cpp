#include "../../include/game/screenoverlay.hpp"


ScreenOverlay::ScreenOverlay(float animation_time, sf::Font& font) {

    this->animation_time = animation_time;
    elapsed_time = 0.0f;
    is_drawn = false;

    rect.setSize( sf::Vector2f(GAME_WIDTH, GAME_HEIGHT * 0.75f) );
    rect.setPosition( sf::Vector2f(0.0f,0.0f) );
    rect.setFillColor( sf::Color(success_r, success_g, success_b, OVERLAY_INIT_ALPHA) );

    success_text.setPosition(350, 150);
    success_text.setFillColor(sf::Color(240,240,240,255));
    success_text.setOutlineColor(sf::Color(20,20,20,255));
    success_text.setOutlineThickness(2.0f);
    success_text.setCharacterSize(25);
    success_text.setFont(font);
    success_text.setString( "SUCCESS!" );

    failure_text.setPosition(350, 150);
    failure_text.setFillColor(sf::Color(240,240,240,255));
    failure_text.setOutlineColor(sf::Color(20,20,20,255));
    failure_text.setOutlineThickness(2.0f);
    failure_text.setCharacterSize(30);
    failure_text.setFont(font);
    failure_text.setString("GAME OVER");
}



void ScreenOverlay::animate(float delta_time) {

    elapsed_time += delta_time;

    if (!is_over) {
        current_alpha = OVERLAY_INIT_ALPHA * (1 - elapsed_time / animation_time );

        if (current_alpha < 0) {
            current_alpha = 0;
            is_drawn = false;
        }


        success_text.setFillColor( sf::Color(240,240,240, current_alpha));
        success_text.setOutlineColor( sf::Color(20,20,20, current_alpha));

        rect.setFillColor ( sf::Color(success_r, success_g, success_b, current_alpha) );




    } /* za szybkie migotanie nie chce mi sie poprawiac aktualnie pewnie wystarczy podzielic elapsed_time :vvv
        else {
        if (elapsed_time > 2*PI)
            elapsed_time -= 2*PI;

        float coeff = fabs( sin(elapsed_time) / 50 );

        int r = (1 - coeff)*fail_r_min + coeff*fail_r_max;
        int g = (1 - coeff)*fail_g_min + coeff*fail_g_max;
        int b = (1 - coeff)*fail_b_min + coeff*fail_b_max;
        // std::cout << coeff << " " << r << " " << g << " " << b << std::endl;
        rect.setFillColor( sf::Color(r,g,b, 255) );

    }*/
}

void ScreenOverlay::draw(sf::RenderWindow& window) {

    if (is_drawn) {
        window.draw(rect);

        if (!is_over)
            window.draw(success_text);
        else
            window.draw(failure_text);
    }
}


void ScreenOverlay::success() {
    elapsed_time = 0;
    is_drawn = true;
}

void ScreenOverlay::gameover() {
    elapsed_time = 0;
    is_over = true;
    is_drawn = true;
    rect.setSize( sf::Vector2f( GAME_WIDTH, GAME_HEIGHT ) );
    rect.setFillColor( sf::Color(fail_r_max, fail_g_max, fail_b_max, 255) );
}