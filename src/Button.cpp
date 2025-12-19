#include "Button.hpp"

void Button::setPosition(float x, float y) {

    sprite.setPosition(x, y);
}

bool Button::isClicked(int mouseX, int mouseY) const {
    
    return sprite.getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY));
}
