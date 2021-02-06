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

    cooldown_text.setPosition(300, 100);
    cooldown_text.setFillColor(sf::Color(240,240,240,255));
    cooldown_text.setOutlineColor(sf::Color(20,20,20,255));
    cooldown_text.setOutlineThickness(2.0f);
    cooldown_text.setCharacterSize(30);
    cooldown_text.setFont(font);
    cooldown_text.setString( "COOLDOWN..." );
}

void Obstacle::generateObstacle() {


    //prevent obstacle draw during generating new obstacle
    std::unique_lock<std::mutex> lock(mtx);

    //generate new obstacle
    obstacle_key = rg.generateMove();

    //set its position accordingly
    if (obstacle_key == 'w')
        rect.setPosition(JUMP_OBSTACLE);
    else if (obstacle_key == 's')
        rect.setPosition(DUCK_OBSTACLE);


    //set obstacle text which will be drawn
    std::string temp_text = "CURRENT KEY: ";
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

}

void Obstacle::draw(sf::RenderWindow& window) {

    
    //prevent obstacle edit during draw phase
    // std::cout << "[OBSTACLE] CD: " << cooldown << std::endl;

    if (cooldown <= 0.0f) {
        std::unique_lock<std::mutex> lock(mtx);
        
        window.draw(rect);
        window.draw(obstacle_text);
    } else {
        window.draw(cooldown_text);
    }


}

void Obstacle::animate(float delta_time) {

    //if on cooldown dont move, dont do shit
    if (cooldown > 0.0f) {
        cooldown -= delta_time;
    } else {
        elapsed_time += delta_time;
    
        move( sf::Vector2f( -GAME_WIDTH* (delta_time / deadline), 0.0f) );
    }
}