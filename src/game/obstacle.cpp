#include "../../include/game/obstacle.hpp"

Obstacle::Obstacle(sf::Font& font) {

    rect.setFillColor(sf::Color(240,240,240,255));
    rect.setOutlineColor(sf::Color(20,20,20,255));
    rect.setOutlineThickness(2.0f);
    rect.setSize(SIZE);

    obstacle_text.setPosition(500, 20);
    obstacle_text.setFillColor(sf::Color(240,240,240,255));
    obstacle_text.setOutlineColor(sf::Color(20,20,20,255));
    obstacle_text.setOutlineThickness(2.0f);
    obstacle_text.setCharacterSize(30);
    obstacle_text.setFont(font);
}

void Obstacle::generateObstacle() {

    // while ( cooldown > 0.0f) {
    //     usleep( 100000 );
    // }
    //prevent obstacle draw during generating new obstacle
    std::unique_lock<std::mutex> lock(mtx);

    //generate new obstacle
    obstacle_key = rg.generateMove();

    //set its position accordingly
    if (obstacle_key == 'w')
        rect.setPosition(DUCK_OBSTACLE);
    else if (obstacle_key == 's')
        rect.setPosition(JUMP_OBSTACLE);

    std::cout << "[OBSTACLE] Generated a '" << obstacle_key << "'\n";

    //set obstacle text which will be drawn
    std::string temp_text = "CURRENT MOVE: ";
    temp_text.push_back(obstacle_key);


    obstacle_text.setString(temp_text);



}


void Obstacle::move(sf::Vector2f vector) {

    std::unique_lock<std::mutex> lock(mtx);
    rect.move(vector);
}

void Obstacle::cleanup(float deadline) {
    this->deadline = deadline;
    elapsed_time = 0.0f;
    obstacle_text.setString(TEXT_BASE);

}

void Obstacle::draw(sf::RenderWindow& window) {

    
    if (cooldown <= 0.0f && obstacle_key != ' ') {
        //prevent obstacle edit during draw phase
        std::unique_lock<std::mutex> lock(mtx);
        
        window.draw(rect);
        window.draw(obstacle_text);
    } else {
        window.draw(obstacle_text);
    }


}

void Obstacle::setCooldown() {
    cooldown = 2000.0f;
    on_cooldown = true;
}

void Obstacle::animate(float delta_time) {

    //if on cooldown dont move, dont do shit

    if (cooldown > 0.0f) {
        cooldown -= delta_time;
        // cooldown_alpha = 255 * (cooldown / 2000.0f);    
        // cooldown_text.setFillColor( sf::Color(240,240,240, cooldown_alpha));
        // cooldown_text.setOutlineColor( sf::Color(20,20,20, cooldown_alpha));
    } else {
        on_cooldown = false;
        elapsed_time += delta_time;
        // std::cout << "[OBSTACLE] Delta: " << delta_time << std::endl;
        // std::cout << "[OBSTACLE] X_vector:" << -GAME_WIDTH* (delta_time / deadline) << std::endl;

        move( sf::Vector2f( -GAME_WIDTH* (delta_time / deadline), 0.0f) );
    }
}