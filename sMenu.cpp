#include "headers/sMenu.h"



sMenu::sMenu(int* points, sf::Vector2f position) :
    points(new int(*points)),
    heart(new sf::Texture())
{
    textfont.loadFromFile("menu_font.ttf");
    pointinfo.setFont(textfont);
    pointinfo.setPosition(position);
    pointinfo.setString("Your score: " + std::to_string(*points));
    pointinfo.setCharacterSize(50);
    pointinfo.setFillColor(sf::Color::Green);
    if (!heart->loadFromFile("images/heart.png"))exit(0);
    for (int i = 0; i < 3; i++) {
        sf::Sprite* temp = new sf::Sprite();
        temp->setTexture(*heart);
        temp->setPosition({ board_size.x - 150 + 50 * i,position.y });
        temp->setScale(0.1f, 0.1f);
        pacmanlives.push_back(temp);
    }
}


void sMenu::drawemenu(sf::RenderWindow& window, int* points) {
    pointinfo.setString("Your score: " + std::to_string(*points));
    window.draw(pointinfo);
    for (int i = 0; i < pacmanlives.size(); i++) {
        if (pacmanlives[i] != nullptr)
            window.draw(*pacmanlives[i]);
    }
}