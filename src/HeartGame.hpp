/* 
 * File:   HeartGame.hpp
 * Author: frex
 *
 * Created on February 4, 2016, 10:07 AM
 */

#ifndef HEARTGAME_HPP
#define	HEARTGAME_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Ring.hpp"
#include "Randomizer.hpp"

class Bullet
{
public:
    bool used = false;
    sf::Vector2f position;
    sf::Vector2f velocity;
    unsigned color;
};

const int kMaxBullets = 50;

class HeartGame
{
public:
    HeartGame(int argc, char ** argv);
    void run();

private:
    void init();

    void update();
    void handleEvents();
    void updateRings();
    void updateBullets();
    void updateHeartAndLevel();

    void draw();
    void drawHeart();
    void drawBrokenHeart();
    void drawBullets();
    void drawText();



    void addBullet();
    void nextLevel();
    void setText(const std::string& text, sf::Time time);

    sf::RenderWindow m_win;
    sf::Font m_font;
    sf::Texture m_hearttex;
    sf::Texture m_brokenhearttex;
    Rings m_rings;
    Bullet m_bullets[kMaxBullets]; //add more?
    Randomizer m_randomizer;
    sf::Clock m_beatclock;
    unsigned m_heartcolor;
    int m_points;
    int m_currentlevelpoints;
    unsigned m_level;
    sf::Clock m_textclock;
    sf::Time m_texttime;
    std::string m_text;
    bool m_gameover;
};

#endif	/* HEARTGAME_HPP */

