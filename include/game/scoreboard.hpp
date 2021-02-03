#include <SFML/Graphics.hpp>


class Scoreboard {

    private:
        unsigned int score{};
        sf::Text score_text;
        

    public:
        Scoreboard(sf::Font& font) {


            score_text.setFont(font);
            score_text.setFillColor(sf::Color(255,255,255,255));
            score_text.setCharacterSize(60);
            score_text.move(400, -20);
        }

        ~Scoreboard() {}
        
        void addPoint() { ++score; }
        
        unsigned int getScore() const { return score; }
        
        void resetScore() { score = 0; }


        void draw(sf::RenderWindow& window) {

            score_text.setString( std::to_string(score) );
            window.draw(score_text);
        }

        void animate(); //to-do

};