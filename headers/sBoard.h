#ifndef sBoard_h
#define sBoard_h
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>


const unsigned int INF = 9999;
const sf::Vector2f board_size = { 999.9f,829.9f };
const sf::Vector2f windows_size = { 999.9f,1000.f };
const float borderthicknes = 25.f;



class sBoard
{
    friend class sPlayer;
    friend class sEnemy;
    friend class sDjikstra;


    sf::Texture AGHtexture;
    sf::Texture foodtexture;

    sf::RectangleShape border[36];
    std::vector<sf::CircleShape*>points;

    float space;
    float border_thicknes;
    float border_thicknes2;
    float foodradious;
    sf::Color foodcolor;
    sf::Vector2f Lettersize;
    sf::Vector2f AGHcoord;


private:
    bool colliding(float radious, sf::Vector2f foodcoords);
public:

    void put_points_on_board();
    void drawing(sf::RenderWindow& window);
    sBoard(sf::Color color, float border_thicknes, float border_thicknes2, sf::Vector2f AGHcoord, sf::Vector2f lettersize, float space);
};

#endif