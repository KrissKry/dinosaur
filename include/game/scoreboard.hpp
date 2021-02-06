#include <SFML/Graphics.hpp>


class Scoreboard {

    private:
        unsigned int score{};
        sf::Text score_text;
        std::string text_content;

    public:
        Scoreboard(sf::Font& font) {


            score_text.setFont(font);
            score_text.setFillColor(sf::Color(255,255,255,255));
            score_text.setOutlineColor(sf::Color(20,20,20,255));
            score_text.setOutlineThickness(2.0f);
            score_text.setCharacterSize(30);
            score_text.move(100, 20);

        }

        ~Scoreboard() {}
        
        void addPoint() { ++score; }
        
        unsigned int getScore() const { return score; }
        
        void resetScore() { score = 0; }


        void draw(sf::RenderWindow& window) {
            
            text_content = "SCORE: ";
            text_content.append( std::to_string(score) );
            
            score_text.setString( text_content );
            window.draw(score_text);
        }

        void animate(); //to-do

};