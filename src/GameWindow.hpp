#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "board.hpp"
#include "Button.hpp"

class GameWindow {
public:
    GameWindow(int windowWidth, int windowHeight, const std::string& playerName, sf::Font& font);
    
    void handleEvent(const sf::Event& event);
    bool shouldShowLeaderboard(int mouseX, int mouseY) const;
    void update();
    void draw(sf::RenderWindow& window);
    bool isGameOn() const { return gameOn; }
    bool isGameUnpaused() const { return gameUnpaused; }
    std::string getPlayerName() const { return playerName; }
    bool getPlayerJustWon() const { return playerJustWon; }
    std::string getLastInsertedName() const { return lastInsertedName; }
    int getLastInsertedTime() const { return lastInsertedTime; }

private:
    board b;
    std::string playerName;
    int windowWidth;
    int windowHeight;
    sf::Font& font;
    
    int mines_remaining;
    bool gameUnpaused;
    bool gameOn;
    bool firstClick;
    bool debugMode;
    int clicks;
    int total_time;
    int accu_time;
    
    bool playerJustWon;
    std::string lastInsertedName;
    int lastInsertedTime;
    
    sf::Clock gameClock;
    
    // Textures
    sf::Texture tile_revealed_texture;
    sf::Texture tile_texture;
    sf::Texture mine_texture;
    sf::Texture flag_texture;
    sf::Texture happy_face_texture;
    sf::Texture dead_face_texture;
    sf::Texture glasses_face_texture;
    sf::Texture debug_texture;
    sf::Texture digits_texture;
    sf::Texture play_texture;
    sf::Texture pause_texture;
    sf::Texture leaderboard_texture;
    std::vector<sf::Texture> numberTextures;
    
    // Sprites
    std::vector<std::vector<sf::Sprite>> revealedSprites;
    std::vector<std::vector<sf::Sprite>> hiddenSprites;
    std::vector<std::vector<sf::Sprite>> flagSprites;
    std::vector<sf::Sprite> mine_counter_sprites;
    
    // Buttons
    Button faceButton;
    Button debugButton;
    Button pausePlayButton;
    Button leaderboardButton;
    
    void loadTextures();
    void initializeSprites();
    void handleLeftClick(int mouseX, int mouseY);
    void handleRightClick(int mouseX, int mouseY);
    void handleButtonClick(int mouseX, int mouseY);
    void checkWinCondition();
    void resetGame();
    void updateLeaderboard();
};

#endif
