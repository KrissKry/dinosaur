#include "../../include/game/obstacle.hpp"

Obstacle::Obstacle(sf::Font& font, sf::Color color) {

    rect.setFillColor(color);
    rect.setSize(SIZE);

    obstacle_text.setPosition(400, 50);
    obstacle_text.setFillColor(sf::Color(240,240,240,255));
    obstacle_text.setCharacterSize(60);
    obstacle_text.setFont(font);
}

void Obstacle::generateObstacle() {

    // std::cout << "[OBSTACLE] Generating..." << std::endl << std::flush;
    //prevent obstacle draw during generating new obstacle
    std::unique_lock<std::mutex> lock(mtx);

    obstacle_key = rg.generateMove();

    if (obstacle_key == 'w')
        rect.setPosition(JUMP_OBSTACLE);
    else if (obstacle_key == 's')
        rect.setPosition(DUCK_OBSTACLE);

    std::string temp_text = "Current key: ";
    temp_text.push_back(obstacle_key);


    obstacle_text.setString(temp_text);

    // std::cout << "[OBSTACLE] Generated a '" << obstacle_key << "'" << std::endl << std::flush;

    // std::unique_lock<std::mutex> unlock(mtx);



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

    // std::unique_lock<std::mutex> unlock(mtx);

}