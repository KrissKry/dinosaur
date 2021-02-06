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


    //prevent obstacle draw during generating new obstacle
    std::unique_lock<std::mutex> lock(mtx);

    obstacle_key = rg.generateMove();


    if (obstacle_key == 'w')
        rect.setPosition(JUMP_OBSTACLE);
    else if (obstacle_key == 's')
        rect.setPosition(DUCK_OBSTACLE);



    std::string temp_text = "CURRENT KEY: ";
    temp_text.push_back(obstacle_key);

    obstacle_text.setString(temp_text);

}


void Obstacle::move(sf::Vector2f vector) {
    rect.move(vector);
}

void Obstacle::cleanup() {
    
}

void Obstacle::draw(sf::RenderWindow& window) {

    
    //prevent obstacle edit during draw phase
    std::unique_lock<std::mutex> lock(mtx);
    
    window.draw(rect);
    window.draw(obstacle_text);


}