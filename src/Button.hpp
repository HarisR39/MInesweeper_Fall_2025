#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button {
    
public:
    sf::Sprite sprite;

    void setPosition(float x, float y);
    bool isClicked(int mouseX, int mouseY) const;
};

#endif
