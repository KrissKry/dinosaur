#include <SFML/Graphics.hpp>

#include "animation.hpp"
#include "gameutil.hpp"

class Sheep {

    private:
        Animation* animation;
        sf::Sprite body;
        sf::Texture texture;

        const std::string sheep_sprite = "../assets/gamedev/owca_sprite.png";

        
        int acceleration_y = 24;

        int velocity_y{};
        bool is_jumping{};
        bool is_ducking{};
        bool collided{};

    public:
        Sheep(float frame_time);
        ~Sheep();


        void draw(sf::RenderWindow& window) { window.draw(body);    }

        void animate(float delta_time);
        void validateMovement(char key);
        void updateMovement();
        void setCollided(bool collided);


        void setPosition(sf::Vector2f vec)  { body.setPosition(vec);}
        void scale(sf::Vector2f vec)        { body.setScale(vec);   }
        void rotate(float degrees)          { body.rotate(degrees); }

        bool isJumping() const { return is_jumping; }
        bool getCollided() const { return collided; }
        sf::Sprite getBody() const { return body;   }
};