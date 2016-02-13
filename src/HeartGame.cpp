#include "HeartGame.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>

HeartGame::HeartGame(int argc, char ** argv)
{
    (void)argc;
    (void)argv;
}

const unsigned kPrimaryColors[] = {
    0xff0000ff,
    0x00ff00ff,
    0x0000ffff,
};

void HeartGame::init()
{
    m_win.create(sf::VideoMode(800u, 600u), "Heart", sf::Style::Titlebar | sf::Style::Close);
    m_win.setFramerateLimit(60u);
    m_font.loadFromFile("read/DejaVuSans.ttf");
    m_hearttex.loadFromFile("read/heart.png");
    m_brokenhearttex.loadFromFile("read/broken-heart.png");
    m_heartcolor = kPrimaryColors[m_randomizer.getRandomUintBelow(3u)];
    m_beatclock.restart();
    m_points = 0;
    m_currentlevelpoints = 0;
    m_gameover = false;
    m_level = 0u;
    nextLevel();
    setText("A and D to rotate\nSpace to shoot\nEscape to quit", sf::seconds(4.f));
}

void HeartGame::run()
{
    init();
    sf::Time time;
    const sf::Time delta = sf::seconds(1.f / 60.f);
    sf::Clock clock;
    while(m_win.isOpen())
    {
        time += clock.restart();
        while(time >= delta)
        {
            update();
            time -= delta;
        }
        draw();
    }//while m win is open
}

void HeartGame::update()
{
    handleEvents();
    if(m_gameover)
        return;

    updateRings();
    updateBullets();
    updateHeartAndLevel();
}

void HeartGame::handleEvents()
{
    sf::Event eve;
    while(m_win.pollEvent(eve))
    {
        switch(eve.type)
        {
            case sf::Event::Closed:
                m_win.close();
                break;
            case sf::Event::KeyPressed:
                if(eve.key.code == sf::Keyboard::Space)
                    addBullet();

                if(eve.key.code == sf::Keyboard::Escape)
                    m_win.close();

                break;
        }//switch eve type
    }//while m win poll event eve
}

void HeartGame::updateRings()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_rings.rotateRings(-0.06f);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_rings.rotateRings(0.06f);
}

void HeartGame::updateBullets()
{
    for(int i = 0; i < kMaxBullets; ++i)
    {
        if(!m_bullets[i].used)
            continue;

        m_bullets[i].position += m_bullets[i].velocity;
        switch(m_rings.checkHit(m_bullets[i].position, m_bullets[i].color))
        {
            case ECHR_HIT:
                m_bullets[i].used = false;
                m_points += 15;
                m_currentlevelpoints += 15;
                break;
            case ECHR_BAD_HIT:
                m_bullets[i].used = false;
                m_points -= 30;
                m_currentlevelpoints -= 30;
                break;
        }

        //out of screen check
        const auto view = m_win.getView();
        sf::FloatRect rect(view.getCenter() - view.getSize() / 2.f, view.getSize());
        rect.top -= 15.f;
        rect.left -= 15.f;
        rect.width += 30.f;
        rect.height += 30.f;
        if(!rect.contains(m_bullets[i].position))
            m_bullets[i].used = false;
    }//for each bullet
}

void HeartGame::updateHeartAndLevel()
{
    if(m_currentlevelpoints < 0)
    {
        m_gameover = true;
        return;
    }

    if(m_rings.isEmpty())
        nextLevel();

    if(m_beatclock.getElapsedTime() >= sf::seconds(1.f))
    {
        unsigned nextcolor = kPrimaryColors[m_randomizer.getRandomUintBelow(3u)];
        while(m_heartcolor == nextcolor)
            nextcolor = kPrimaryColors[m_randomizer.getRandomUintBelow(3u)];

        m_heartcolor = nextcolor;
        m_beatclock.restart();
    }
}

template <typename T>
static inline void setOriginToMiddle(T& t)
{
    const sf::FloatRect r = t.getLocalBounds();
    t.setOrigin((r.left + r.width) / 2.f, (r.top + r.height) / 2.f);
}

static inline float calcPointScale(int points, int maxpoints)
{
    const float lerp01 = std::max(0.f, std::min(points / (float)maxpoints, 1.f));
    return (1.f - lerp01) * 0.5f + lerp01 * 2.f;
}

void HeartGame::draw()
{
    m_win.clear();
    if(m_gameover)
    {
        drawBrokenHeart();
    }
    else
    {
        drawHeart();
    }
    m_win.draw(m_rings);
    drawBullets();
    drawText();
    m_win.display();
}

void HeartGame::drawHeart()
{
    sf::Sprite spr(m_hearttex);
    setOriginToMiddle(spr);
    spr.setPosition(400.f, 300.f);
    spr.setColor(sf::Color(m_heartcolor));
    const float beatscale = 0.66f + 0.33f * (1.f - m_beatclock.getElapsedTime().asSeconds()) / 1.f;
    const float pointscale = calcPointScale(m_currentlevelpoints, m_rings.getMaxPoints());
    spr.setScale(beatscale * pointscale, beatscale * pointscale);
    m_win.draw(spr);

    sf::Text text(std::to_string(m_points), m_font, 20u);
    setOriginToMiddle(text);
    text.setPosition(spr.getPosition());
    text.move(0.f, -55.f - (text.getLocalBounds().top + text.getLocalBounds().height) / 2.f);
    m_win.draw(text);
}

void HeartGame::drawBrokenHeart()
{
    sf::Sprite spr(m_brokenhearttex);
    setOriginToMiddle(spr);
    spr.setPosition(400.f, 300.f);
    spr.setColor(sf::Color(m_heartcolor));
    m_win.draw(spr);

    sf::Text text(std::to_string(m_points), m_font, 20u);
    setOriginToMiddle(text);
    text.setPosition(spr.getPosition());
    text.move(0.f, -55.f - (text.getLocalBounds().top + text.getLocalBounds().height) / 2.f);
    m_win.draw(text);

    text.setString("BROKEN");
    text.setStyle(sf::Text::Bold);
    setOriginToMiddle(text);
    text.setPosition(spr.getPosition());
    m_win.draw(text);
}

void HeartGame::drawBullets()
{
    sf::CircleShape sha;
    sha.setRadius(5.f);
    sha.setScale(1.f, 1.f);
    sha.setOutlineThickness(-1.f);
    sha.setOutlineColor(sf::Color::Black);
    sha.setOrigin(sha.getRadius(), sha.getRadius());
    for(int i = 0; i < kMaxBullets; ++i)
    {
        if(m_bullets[i].used)
        {
            sha.setFillColor(sf::Color(m_bullets[i].color));
            sha.setPosition(m_bullets[i].position);
            m_win.draw(sha);
        }
    }
}

void HeartGame::drawText()
{
    if(m_textclock.getElapsedTime() < m_texttime)
    {
        sf::Text text(m_text, m_font, 20u);
        sf::Color c = sf::Color::White;
        c.a = (1.f - (m_textclock.getElapsedTime() / m_texttime)) * 255.f;
        text.setColor(c);
        m_win.draw(text);
    }

}

void HeartGame::addBullet()
{
    if(m_gameover)
        return;

    for(int i = 0; i < kMaxBullets; ++i)
    {
        if(!m_bullets[i].used)
        {
            m_bullets[i].position = sf::Vector2f(400.f, 300.f);
            m_bullets[i].velocity = m_randomizer.randomVectorFromLength(3.f);
            m_bullets[i].color = m_heartcolor;
            m_bullets[i].used = true;
            break;
        }
    }
}

void HeartGame::nextLevel()
{
    ++m_level;
    m_rings.create(m_randomizer, std::min(3u, m_level));
    for(int i = 0; i < kMaxBullets; ++i)
        m_bullets[i].used = false;

    m_currentlevelpoints = 0;
    setText("Level " + std::to_string(m_level), sf::seconds(1.5f));
}

void HeartGame::setText(const std::string& text, sf::Time time)
{
    m_text = text;
    m_texttime = time;
    m_textclock.restart();
}
