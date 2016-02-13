/* 
 * File:   Randomizer.hpp
 * Author: frex
 *
 * Created on February 7, 2016, 8:52 AM
 */

#ifndef RANDOMIZER_HPP
#define	RANDOMIZER_HPP

#include <random>
#include <SFML/System/Vector2.hpp>

class Randomizer
{
public:

    Randomizer();
    sf::Vector2f randomVectorFromLength(float length);
    unsigned getRandomUintBelow(unsigned exclusivemax);

    //a ring where two next elements are not equal
    void createRandomRing(unsigned * buff, unsigned count, unsigned exclusivemax);

private:
    std::mt19937 m_mt;
    std::vector<unsigned> m_colors;

};

#endif	/* RANDOMIZER_HPP */

