#include"Button.h"

Button::Button(sf::Texture *texture,sf::Vector2f initial,int x,int y){
    pozitie=initial;
    maxX=x;
    maxY=y;
    butt.setSize(sf::Vector2f(float(maxX),float(maxY)));
    butt.setTexture(texture);
    butt.setPosition(pozitie);
    }
Button::~Button(){
}

void Button::Draw(sf::RenderWindow &window){
    window.draw(butt);
}
bool Button::Check(sf::RenderWindow &window){
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        sf::Vector2i pos=sf::Mouse::getPosition(window);
        if(pos.x>pozitie.x&&pos.x<pozitie.x+maxX&&pos.y>pozitie.y&&pos.y<pozitie.y+maxY)
            return true;
    }
    return false;
}

