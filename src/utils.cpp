#include "utils.hpp"

std::vector<sf::Sprite> calcMinesRemaining(int& mines_remaining, sf::Texture& texture, const board& b) {

    int hundreds = (mines_remaining / 100) % 10;
    int tens = (mines_remaining / 10) % 10;
    int ones = mines_remaining % 10;

    std::vector<int> temp = {hundreds, tens, ones};
    std::vector<sf::Sprite> sprites;
    sprites.resize(3);

    for(int i = 0; i < 3; i++) {

        sprites[i].setTexture(texture);
        sprites[i].setTextureRect(sf::IntRect(temp[i] * 21, 0, 21, 32));
        sprites[i].setPosition(33 + i*21, b.rows * 32 + 16);
    }

    return sprites;
}

std::vector<sf::Sprite> calcTime(int time, sf::Texture& texture, const board& b) {

    int minutes_tens = (time/600);
    int minutes_ones = (time - minutes_tens * 600)/60;
    int seconds_tens = (time - (minutes_tens * 600) - (minutes_ones * 60))/10;
    int seconds_ones = (time - (minutes_tens * 600) - (minutes_ones * 60) - (seconds_tens * 10));

    std::vector<int> temp = {minutes_tens, minutes_ones, seconds_tens, seconds_ones};
    std::vector<sf::Sprite> sprites;
    sprites.resize(4);

    for(int i = 0; i < 2; i++) {

        sprites[i].setTexture(texture);
        sprites[i].setTextureRect(sf::IntRect(temp[i] * 21, 0, 21, 32));
        sprites[i].setPosition(b.cols*32 - 97 + 21*i, b.rows * 32 + 16);
    }

    for(int i = 2; i < 4; i++) {

        sprites[i].setTexture(texture);
        sprites[i].setTextureRect(sf::IntRect(temp[i] * 21, 0, 21, 32));
        sprites[i].setPosition(b.cols*32 - 54 + 21*(i-2), b.rows * 32 + 16);
    }

    return sprites;
}

void setText(sf::Text &text, float x, float y) {
    
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
