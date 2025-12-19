#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "tile.hpp"

class board {
    
public:
    int mines;
    int rows;
    int cols;
    std::vector<std::vector<tile>> grid;
    std::vector<int> mineLoc;

    const tile& gridAt(int r, int c) const;
    void loadConfig(const std::string& path);
    void numberLogic();
    void revealLogic(
        int r, int c, int& clicks,
        std::vector<std::vector<sf::Sprite>>& hiddenSprites,
        const sf::Texture& tile_revealed_texture,
        const std::vector<sf::Texture>& numberTextures);
};

#endif
