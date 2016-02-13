#include "Randomizer.hpp"
#include <cmath>
#include <ctime>
#include <cassert>

Randomizer::Randomizer() : m_mt((unsigned)std::time(0x0)) { }

sf::Vector2f Randomizer::randomVectorFromLength(float length)
{
    const float pi2 = 2.f * 3.14159265f;
    const float a = std::uniform_real_distribution<float>(0.f, pi2)(m_mt);
    return sf::Vector2f(length * std::cos(a), length * std::sin(a));
}

unsigned Randomizer::getRandomUintBelow(unsigned exclusivemax)
{
    if(exclusivemax == 0u)
        return 0u;

    std::uniform_int_distribution<unsigned> dist(0u, exclusivemax - 1u);
    return dist(m_mt);
}

void Randomizer::createRandomRing(unsigned * buff, unsigned count, unsigned exclusivemax)
{
    if(exclusivemax < 3 || count < 3)
    {
        for(unsigned i = 0u; i < count; ++i)
            buff[i] = getRandomUintBelow(exclusivemax);

        return;
    }

    std::uniform_int_distribution<unsigned> dist(0u, exclusivemax - 1u);
    buff[0] = dist(m_mt);
    buff[count - 1] = dist(m_mt);
    while(buff[0] == buff[count - 1])
        buff[count - 1] = dist(m_mt);

    for(unsigned i = 1u; i < (count - 1); ++i)
    {
        buff[i] = dist(m_mt);
        while(buff[i] == buff[i - 1] || buff[i] == buff[(i + 1) % count])
            buff[i] = dist(m_mt);
    }
}
