#ifndef sMenu_h
#define sMenu_h 


#include "sBoard.h"



class sMenu
{
public:
    sMenu(int* points, sf::Vector2f position);
    sf::Text pointinfo;
    sf::Font textfont;
    int* points;
    sf::Texture* heart;
    std::vector<sf::Sprite*>pacmanlives;
    void drawemenu(sf::RenderWindow& window, int* points);
};


#endif