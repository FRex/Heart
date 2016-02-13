/* 
 * File:   Ring.hpp
 * Author: frex
 *
 * Created on February 6, 2016, 3:09 AM
 */

#ifndef RING_HPP
#define	RING_HPP

#include <vector>
#include <SFML/Graphics/Drawable.hpp>

class RingSegment
{
public:
    float startAngle;
    float endAngle;
    unsigned color;
    float innerRadius;
    float outerRadius;
};

class Randomizer;

enum ECHECK_HIT_RESULT
{
    //bullet hit into right segment
    ECHR_HIT,

    //bullet hit but into wrong segment
    ECHR_BAD_HIT,

    //bullet didn't hit any ring segment
    ECHR_MISS
};

class Rings : public sf::Drawable
{
public:
    void rotateRings(float amount);
    void create(Randomizer& randomizer, unsigned count);
    ECHECK_HIT_RESULT checkHit(sf::Vector2f p, unsigned color);
    int getMaxPoints() const;
    bool isEmpty() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void createRing(float a, float r, float R, Randomizer& randomizer);

    float m_rotation = 100000.f;
    std::vector<RingSegment> m_segments;
    int m_maxpoints = 0;
};


#endif	/* RING_HPP */

