#include <SFML/Graphics.hpp>

class CollisionChecker {

    private:

    public:
        CollisionChecker() {}
        ~CollisionChecker() {}

        bool checkCollision( const sf::Sprite& spr, const sf::RectangleShape& rect) {

            float left = rect.getGlobalBounds().left;
            float top = rect.getGlobalBounds().top;
            float height = rect.getGlobalBounds().height;

            //if sprite contains rectangle topleft corner
            if ( spr.getGlobalBounds().contains( sf::Vector2f( left, top  )))
                return true;
            
            //if sprite contains rectangle botleft corner
            if ( spr.getGlobalBounds().contains( sf::Vector2f( left, top + height)))
                return true;

            //if rectangle is 
            // if ( spr.getGlobalBounds().left >= left)
                // return true;

            return false;
        }
};