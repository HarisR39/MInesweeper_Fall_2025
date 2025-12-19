#ifndef LEADERBOARDWINDOW_HPP
#define LEADERBOARDWINDOW_HPP

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class LeaderboardWindow {
public:
    LeaderboardWindow(int windowWidth, int windowHeight, sf::Font& font);
    
    void show(const std::string& lastInsertedName = "", int lastInsertedTime = 0, bool playerJustWon = false, 
              int mainWindowX = 0, int mainWindowY = 0);
    void run();
    bool isOpen() const { return window.isOpen(); }
    void close() { window.close(); }

private:
    sf::RenderWindow window;
    sf::Font& font;
    int windowWidth;
    int windowHeight;
    sf::Text leaderboardTitle;
    sf::Text leaderboardText;
    
    std::vector<std::pair<std::string, int>> loadLeaderboard();
    std::string formatLeaderboardContent(const std::vector<std::pair<std::string, int>>& leaderboard,
                                        const std::string& lastInsertedName, int lastInsertedTime, bool playerJustWon);
};

#endif
