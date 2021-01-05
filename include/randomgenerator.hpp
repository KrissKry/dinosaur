#ifndef SCZR_RANDOMGENERATOR_H
#define SCZR_RANDOMGENERATOR_H
#pragma once
#include <random>
enum class Key_Press {
    W = (int)'w',
    S = (int)'s',
    NON = (int)' '
};



class RandomGenerator {

    private:

        std::mt19937 m_mt;
        char moves[3] = {'w', 's', ' '};
    public:

        RandomGenerator() : m_mt( (std::random_device()) ()) {} //inits marsenne-twister rg
        ~RandomGenerator() {}


        char generateMove() {
            std::uniform_int_distribution<int> dist(0,2);

            // std::cout <<
            // return (char)(Key_Press( dist(m_mt) ));
            return moves[ dist(m_mt) ];
        }
};



#endif