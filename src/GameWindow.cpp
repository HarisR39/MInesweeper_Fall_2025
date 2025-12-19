#include "GameWindow.hpp"
#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

GameWindow::GameWindow(int windowWidth, int windowHeight, const std::string& playerName, sf::Font& font)
    : playerName(playerName), windowWidth(windowWidth), windowHeight(windowHeight), font(font),
      gameUnpaused(true), gameOn(true), firstClick(true), debugMode(false), clicks(0),
      total_time(0), accu_time(0), playerJustWon(false) {
    
    b.loadConfig("../files/config.cfg");
    b.numberLogic();
    
    mines_remaining = b.mines;
    
    loadTextures();
    initializeSprites();
}

void GameWindow::loadTextures() {

    tile_revealed_texture.loadFromFile("../files/images/tile_revealed.png");
    tile_texture.loadFromFile("../files/images/tile_hidden.png");
    mine_texture.loadFromFile("../files/images/mine.png");
    flag_texture.loadFromFile("../files/images/flag.png");
    happy_face_texture.loadFromFile("../files/images/face_happy.png");
    dead_face_texture.loadFromFile("../files/images/face_lose.png");
    glasses_face_texture.loadFromFile("../files/images/face_win.png");
    debug_texture.loadFromFile("../files/images/debug.png");
    digits_texture.loadFromFile("../files/images/digits.png");
    play_texture.loadFromFile("../files/images/play.png");
    pause_texture.loadFromFile("../files/images/pause.png");
    leaderboard_texture.loadFromFile("../files/images/leaderboard.png");
    
    numberTextures.resize(9);
    for (int i = 1; i <= 8; i++) {

        std::string path = "../files/images/number_" + std::to_string(i) + ".png";
        numberTextures[i].loadFromFile(path);
    }
}

void GameWindow::initializeSprites() {

    revealedSprites.resize(b.rows, std::vector<sf::Sprite>(b.cols));
    hiddenSprites.resize(b.rows, std::vector<sf::Sprite>(b.cols));
    flagSprites.resize(b.rows, std::vector<sf::Sprite>(b.cols));
    
    for (int r = 0; r < b.rows; r++) {
        for (int c = 0; c < b.cols; c++) {
            flagSprites[r][c].setTexture(flag_texture);
            flagSprites[r][c].setPosition(c * 32, r * 32);
            
            revealedSprites[r][c].setTexture(tile_revealed_texture);
            revealedSprites[r][c].setPosition(c * 32, r * 32);
            
            hiddenSprites[r][c].setTexture(tile_texture);
            hiddenSprites[r][c].setPosition(c * 32, r * 32);
        }
    }
    
    faceButton.sprite.setTexture(happy_face_texture);
    faceButton.setPosition(windowWidth/2 - 32, b.rows * 32 + 16);
    
    debugButton.sprite.setTexture(debug_texture);
    debugButton.setPosition(windowWidth - 304, b.rows * 32 + 16);
    
    pausePlayButton.sprite.setTexture(pause_texture);
    pausePlayButton.setPosition(windowWidth - 240, b.rows * 32 + 16);
    
    leaderboardButton.sprite.setTexture(leaderboard_texture);
    leaderboardButton.setPosition(windowWidth - 176, b.rows * 32 + 16);
    
    mine_counter_sprites = calcMinesRemaining(mines_remaining, digits_texture, b);
}

void GameWindow::handleEvent(const sf::Event& event) {

    if (event.type == sf::Event::MouseButtonPressed) {

        if (event.mouseButton.button == sf::Mouse::Left) {

            int mouseX = event.mouseButton.x;
            int mouseY = event.mouseButton.y;
            
            if(mouseY < b.rows * 32) {

                handleLeftClick(mouseX, mouseY);
            }

            else {
                handleButtonClick(mouseX, mouseY);
            }
        } 

        else if(event.mouseButton.button == sf::Mouse::Right && gameUnpaused && gameOn) {

            int mouseX = event.mouseButton.x;
            int mouseY = event.mouseButton.y;
            handleRightClick(mouseX, mouseY);
        }
    }
}

void GameWindow::handleLeftClick(int mouseX, int mouseY) {

    int r = mouseY/32;
    int c = mouseX/32;
    
    if (firstClick) {
        while (b.grid[r][c].isBomb || b.grid[r][c].adjMines != 0) {

            b = board();
            b.loadConfig("../files/config.cfg");
            b.numberLogic();
        }
        
        hiddenSprites[r][c].setTexture(tile_revealed_texture);
        b.revealLogic(r, c, clicks, hiddenSprites, tile_revealed_texture, numberTextures);
        firstClick = false;

    }

    else if (b.grid[r][c].adjMines == 0 && !(b.grid[r][c].isBomb) && gameUnpaused && gameOn && !(b.grid[r][c].isFlagged)){
        hiddenSprites[r][c].setTexture(tile_revealed_texture);
        b.revealLogic(r, c, clicks, hiddenSprites, tile_revealed_texture, numberTextures);
    } 

    else if (b.grid[r][c].isBomb && gameUnpaused && !(b.grid[r][c].isFlagged) && gameOn) {
        hiddenSprites[r][c].setTexture(mine_texture);
        b.grid[r][c].isClicked = true;
        faceButton.sprite.setTexture(dead_face_texture);
        gameOn = false;
        gameUnpaused = false;
        for (int i : b.mineLoc) {

            int r = i / b.cols;
            int c = i % b.cols;
            hiddenSprites[r][c].setTexture(mine_texture);
        }
        
        accu_time += gameClock.getElapsedTime().asSeconds();
    } 

    else if (gameUnpaused && !(b.grid[r][c].isFlagged) && gameOn) {
        hiddenSprites[r][c].setTexture(numberTextures[b.grid[r][c].adjMines]);
        b.grid[r][c].isClicked = true;
        clicks++;
    }
    
    checkWinCondition();
}

void GameWindow::handleRightClick(int mouseX, int mouseY) {

    if(mouseY < b.rows * 32) {
        int r = mouseY/32;
        int c = mouseX/32;
        
        if (hiddenSprites[r][c].getGlobalBounds().contains(mouseX, mouseY) && !(b.grid[r][c].isClicked) && !(b.grid[r][c].isFlagged) && mines_remaining > 0) {
            b.grid[r][c].isFlagged = true;
            mines_remaining--;
            mine_counter_sprites = calcMinesRemaining(mines_remaining, digits_texture, b);
        } 

        else if (hiddenSprites[r][c].getGlobalBounds().contains(mouseX, mouseY) && !(b.grid[r][c].isClicked) && b.grid[r][c].isFlagged) {
            b.grid[r][c].isFlagged = false;
            mines_remaining++;
            mine_counter_sprites = calcMinesRemaining(mines_remaining, digits_texture, b);
        }
    }
}

void GameWindow::handleButtonClick(int mouseX, int mouseY){

    if(faceButton.isClicked(mouseX, mouseY)){

        resetGame();
    } 

    else if(debugButton.isClicked(mouseX, mouseY) && gameUnpaused && gameOn) {

        if(!debugMode) {

            for (int i : b.mineLoc) {

                int r = i / b.cols;
                int c = i % b.cols;
                hiddenSprites[r][c].setTexture(mine_texture);
            }

            debugMode = true;

        } 
        
        else {

            for (int i : b.mineLoc) {

                int r = i / b.cols;
                int c = i % b.cols;
                hiddenSprites[r][c].setTexture(tile_texture);
            }

            debugMode = false;
        }

    } 
    
    else if(pausePlayButton.isClicked(mouseX, mouseY) && gameUnpaused && gameOn) {
        
        gameUnpaused = false;
        pausePlayButton.sprite.setTexture(play_texture);
        accu_time += gameClock.getElapsedTime().asSeconds();
    } 
    
    else if(pausePlayButton.isClicked(mouseX, mouseY) && !gameUnpaused && gameOn) {
        
        gameUnpaused = true;
        pausePlayButton.sprite.setTexture(pause_texture);
        gameClock.restart();
    }
}

void GameWindow::checkWinCondition() {

    if(clicks == b.rows * b.cols - b.mines) {

        int final_time = accu_time + gameClock.getElapsedTime().asSeconds();
        
        gameOn = false;
        gameUnpaused = false;
        faceButton.sprite.setTexture(glasses_face_texture);

        for (int i : b.mineLoc) {

            int r = i / b.cols;
            int c = i % b.cols;
            b.grid[r][c].isFlagged = true;
        }
        
        updateLeaderboard();
    }
}

void GameWindow::updateLeaderboard() {

    std::string playerName = this->playerName;
    if(!playerName.empty() && playerName.back() == '|') playerName.pop_back();
    if(playerName.empty()) playerName = "Anon";
    
    int final_time = accu_time + gameClock.getElapsedTime().asSeconds();
    
    std::vector<std::pair<std::string,int>> entries;
    std::ifstream ifile("../files/leaderboard.txt");
    std::string line;
    
    while (std::getline(ifile, line)) {

        if(line.empty()) continue;
        size_t commaPos = line.find(',');
        if(commaPos == std::string::npos) continue;
        
        std::string left = line.substr(0, commaPos);
        std::string right = line.substr(commaPos + 1);
        std::string name;
        int t = 0;
        
        if (left.find(':') != std::string::npos) {

            int mm = 0, ss = 0; char colon;
            std::istringstream ts(left);
            if (ts >> mm >> colon >> ss) t = mm * 60 + ss;
            name = right;
        } 

        else if (right.find(':') != std::string::npos) {

            int mm = 0, ss = 0; char colon;
            std::istringstream ts(right);
            if (ts >> mm >> colon >> ss) t = mm * 60 + ss;
            name = left;
        } 
        
        else {

            std::istringstream ts(right);
            if (ts >> t) {
                name = left;
            } 
            
            else {

                std::istringstream ts2(left);
                if (ts2 >> t) {

                    name = right;
                } 
                
                else {

                    name = left;
                    t = 0;
                }
            }
        }

        entries.push_back({name, t});
    }

    ifile.close();
    
    std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b){ return a.second < b.second; });
    
    bool inserted = false;

    if(entries.size() < 5) {

        entries.push_back({playerName, final_time});
        inserted = true;
    } 

    else {

        if(final_time < entries.back().second) {

            entries.push_back({playerName, final_time});
            inserted = true;
        }
    }
    
    if(inserted) {

        std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b){ return a.second < b.second; });
        if(entries.size() > 5) entries.resize(5);
        
        std::ofstream ofile("../files/leaderboard.txt", std::ofstream::trunc);
        for(const auto &p : entries) {
            int mm = p.second / 60;
            int ss = p.second % 60;
            if (mm < 10) ofile << '0';
            ofile << mm << ':';
            if (ss < 10) ofile << '0';
            ofile << ss << ',' << p.first << '\n';
        }

        ofile.close();
        
        playerJustWon = true;
        lastInsertedName = playerName;
        lastInsertedTime = final_time;
    }
}

void GameWindow::resetGame() {

    b = board();
    b.loadConfig("../files/config.cfg");
    b.numberLogic();
    
    mines_remaining = b.mines;
    mine_counter_sprites = calcMinesRemaining(mines_remaining, digits_texture, b);
    faceButton.sprite.setTexture(happy_face_texture);
    pausePlayButton.sprite.setTexture(pause_texture);
    gameClock.restart();
    gameUnpaused = true;
    gameOn = true;
    firstClick = true;
    clicks = 0;
    total_time = 0;
    accu_time = 0;
    
    for (int r = 0; r < b.rows; r++) {
        for (int c = 0; c < b.cols; c++) {
            b.grid[r][c].isClicked = false;
            b.grid[r][c].isFlagged = false;
            hiddenSprites[r][c].setTexture(tile_texture);
        }
    }
}

void GameWindow::update() {

    sf::Time t = gameClock.getElapsedTime();
    int seconds = t.asSeconds();
    if(gameUnpaused) {
        total_time = accu_time + seconds;
    } else {
        total_time = accu_time;
    }
}

void GameWindow::draw(sf::RenderWindow& window) {

    window.clear(sf::Color::White);
    
    for (int r = 0; r < b.rows; r++) {
        for (int c = 0; c < b.cols; c++) {
            window.draw(revealedSprites[r][c]);
            window.draw(hiddenSprites[r][c]);
            
            if(b.grid[r][c].isFlagged) {
                window.draw(flagSprites[r][c]);
            }
        }
    }
    
    window.draw(faceButton.sprite);
    window.draw(debugButton.sprite);
    window.draw(pausePlayButton.sprite);
    window.draw(leaderboardButton.sprite);
    
    std::vector<sf::Sprite> timer_sprite = calcTime(total_time, digits_texture, b);

    for(sf::Sprite sprite : mine_counter_sprites) {

        window.draw(sprite);
    }

    for(sf::Sprite sprite : timer_sprite) {

        window.draw(sprite);
    }
}

bool GameWindow::shouldShowLeaderboard(int mouseX, int mouseY) const {
    
    return leaderboardButton.isClicked(mouseX, mouseY);
}
