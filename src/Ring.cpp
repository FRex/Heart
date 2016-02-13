#include "Ring.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include "Randomizer.hpp"

const float pi = 3.14159265;

void Rings::rotateRings(float amount)
{
    m_rotation += amount;

    //make sure its positive
    while(m_rotation < pi)
        m_rotation += 10.f * pi;
}

void Rings::create(Randomizer& randomizer, unsigned count)
{
    m_segments.clear();

    switch(count)
    {
        case 3u:
            createRing(0.f, 260.f, 291.f, randomizer);
        case 2u:
            createRing(0.5f * pi, 230.f, 261.f, randomizer);
        case 1u:
            createRing(0.f, 200.f, 231.f, randomizer);
            break;
    }

    m_maxpoints = 15 * m_segments.size();
}

static inline float len(sf::Vector2f v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

static inline float anglediff(float a, float b)
{
    return std::abs(std::atan2(std::sin(a - b), std::cos(a - b)));
}

static inline bool between(float x, float a, float b)
{
    return (anglediff(a, x) + anglediff(x, b)) <= anglediff(a, b);
}

static inline float angle(sf::Vector2f v)
{
    return std::atan2(v.y, v.x);
}

ECHECK_HIT_RESULT Rings::checkHit(sf::Vector2f p, unsigned color)
{
    p -= sf::Vector2f(400.f, 300.f);
    const float l = len(p);
    for(auto it = m_segments.begin(); it != m_segments.end(); ++it)
    {
        if(!(it->innerRadius < l && l < it->outerRadius))
            continue;

        if(!between(angle(p), m_rotation + it->startAngle, m_rotation + it->endAngle))
            continue;

        //check if there is hit on any bit except for alpha channel
        if(((color & it->color) >> 8) != 0u)
        {
            m_segments.erase(it);
            return ECHR_HIT;
        }
        else
        {
            return ECHR_BAD_HIT;
        }
    }
    return ECHR_MISS;
}

int Rings::getMaxPoints() const
{
    return m_maxpoints;
}

static inline sf::Vector2f pt(sf::Vector2f a, float r, float d)
{
    return sf::Vector2f(a.x + r * std::cos(d), a.y + r * std::sin(d));
}

static void drawRingSegment(sf::RenderTarget& target, sf::Vector2f p, float r, float R, float a, float b, unsigned c)
{
    sf::VertexArray arr(sf::TrianglesStrip);

    arr.append(sf::Vertex(pt(p, r, a), sf::Color(c)));
    arr.append(sf::Vertex(pt(p, R, a), sf::Color(c)));

    const int pts = 11;
    for(int i = 0; i < pts; ++i)
    {
        const float cr = (i % 2 == 0)?r:R;
        const float s = a + (i + 1) * (b - a) / (pts + 1);
        arr.append(sf::Vertex(pt(p, cr, s), sf::Color(c)));
    }
    arr.append(sf::Vertex(pt(p, R, b), sf::Color(c)));
    arr.append(sf::Vertex(pt(p, r, b), sf::Color(c)));
    target.draw(arr);
}

void Rings::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(const auto& seg : m_segments)
    {
        drawRingSegment(target, sf::Vector2f(400.f, 300.f), seg.innerRadius, seg.outerRadius, m_rotation + seg.startAngle, m_rotation + seg.endAngle, seg.color);
    }
}

bool Rings::isEmpty() const
{
    return m_segments.empty();
}

const unsigned kColors[7] = {
    0xff0000ff,
    0x00ff00ff,
    0x0000ffff,

    0xffff00ff,
    0xff00ffff,
    0x00ffffff,

    0xffffffff,
};

void Rings::createRing(float a, float r, float R, Randomizer& randomizer)
{
    unsigned rand[6];
    randomizer.createRandomRing(rand, 6u, 7u);
    for(int i = 0; i < 6; ++i)
    {
        RingSegment seg;
        seg.startAngle = a + i * pi / 3.f;
        seg.endAngle = a + (i + 1) * pi / 3.f;
        seg.innerRadius = r;
        seg.outerRadius = R;
        seg.color = kColors[rand[i]];
        m_segments.push_back(seg);
    }
}
