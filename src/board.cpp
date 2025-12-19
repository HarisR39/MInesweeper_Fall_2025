#include "board.hpp"
#include <fstream>
#include <random>
#include <algorithm>

const tile& board::gridAt(int r, int c) const {

    return grid[r][c];
}

void board::loadConfig(const std::string& path) {

    std::ifstream in(path);
    in >> cols;
    in >> rows;
    in >> mines;

    grid.resize(rows, std::vector<tile>(cols));

    mineLoc.clear();
    for (int i = 0; i <= cols*rows-1; ++i) {
        mineLoc.push_back(i);
    }

    static std::mt19937 rng(std::random_device{}());
    std::shuffle(mineLoc.begin(), mineLoc.end(), rng);
    mineLoc.resize(mines);

    for (int i : mineLoc) {

        int r = i / cols;
        int c = i % cols;
        grid[r][c].isBomb = true;
    }
}

void board::numberLogic() {

    auto inBounds = [&](int rr, int cc) {

        return rr >= 0 && rr < rows && cc >= 0 && cc < cols;
    };

    int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int r = 0; r < rows; r++) {

        for (int c = 0; c < cols; c++) {

            if (grid[r][c].isBomb) {
                grid[r][c].adjMines = 0;  
                continue; 
            }

            int count = 0;
            for (int k = 0; k < 8; k++) {

                int nr = r + dr[k];
                int nc = c + dc[k];

                if (inBounds(nr, nc) && grid[nr][nc].isBomb) {

                    count++;
                }
            }

            grid[r][c].adjMines = count;
        }
    }
}

void board::revealLogic(

    int r, int c, int& clicks,
    std::vector<std::vector<sf::Sprite>>& hiddenSprites,
    const sf::Texture& tile_revealed_texture,
    const std::vector<sf::Texture>& numberTextures) {

    auto inBounds = [&](int rr, int cc) {

        return rr >= 0 && rr < rows && cc >= 0 && cc < cols;
    };

    int dr[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[8] = {-1,  0,  1,-1, 1,-1, 0, 1};

    if (!inBounds(r, c)) return;
    if (grid[r][c].isClicked) return;
    if (grid[r][c].isBomb) return;

    grid[r][c].isClicked = true;

    if (grid[r][c].adjMines == 0) {

        hiddenSprites[r][c].setTexture(tile_revealed_texture);
        clicks++;
    } 
    else {

        hiddenSprites[r][c].setTexture(numberTextures[grid[r][c].adjMines]);
        clicks++;
    }

    if (grid[r][c].adjMines != 0) {return;}

    for (int k = 0; k < 8; ++k) {
        
        int nr = r + dr[k];
        int nc = c + dc[k];

        if (!inBounds(nr, nc)) continue;
        if (grid[nr][nc].isClicked) continue;
        if (grid[nr][nc].isBomb) continue;

        revealLogic(nr, nc, clicks, hiddenSprites, tile_revealed_texture, numberTextures);
    }
}
