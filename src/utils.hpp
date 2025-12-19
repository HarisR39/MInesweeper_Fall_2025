#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "board.hpp"

std::vector<sf::Sprite> calcMinesRemaining(int& mines_remaining, sf::Texture& texture, const board& b);
std::vector<sf::Sprite> calcTime(int time, sf::Texture& texture, const board& b);
void setText(sf::Text &text, float x, float y);

#endif
