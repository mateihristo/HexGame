#pragma once
#include"SFML/Graphics.hpp"

class Button{
public:
    Button(sf::Texture *texture,sf::Vector2f initial,int x,int y);
    ~Button();

    void Draw(sf::RenderWindow& window);
    bool Check(sf::RenderWindow &window);

private:
    sf::Vector2f pozitie;
    int maxX,maxY;
    sf::RectangleShape butt;



};

