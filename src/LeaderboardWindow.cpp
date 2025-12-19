#include "LeaderboardWindow.hpp"
#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

LeaderboardWindow::LeaderboardWindow(int windowWidth, int windowHeight, sf::Font& font)
    : windowWidth(windowWidth), windowHeight(windowHeight), font(font) {
    
    leaderboardTitle.setFont(font);
    leaderboardTitle.setString("LEADERBOARD");
    leaderboardTitle.setCharacterSize(20);
    leaderboardTitle.setFillColor(sf::Color::White);
    leaderboardTitle.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(leaderboardTitle, windowWidth/2, windowHeight/2 - 120);

    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(18);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setStyle(sf::Text::Bold);
}

void LeaderboardWindow::show(const std::string& lastInsertedName, int lastInsertedTime, bool playerJustWon,
                             int mainWindowX, int mainWindowY) {
    if (!window.isOpen()) {
        window.create(sf::VideoMode(windowWidth, windowHeight), "Leaderboard", sf::Style::Close);
        int lx = mainWindowX;
        int ly = mainWindowY - 40;
        if(ly < 0) ly = 0;
        window.setPosition(sf::Vector2i(lx, ly));
    }
    
    std::vector<std::pair<std::string, int>> leaderboard = loadLeaderboard();
    std::string content = formatLeaderboardContent(leaderboard, lastInsertedName, lastInsertedTime, playerJustWon);
    
    leaderboardText.setString(content);
    setText(leaderboardText, windowWidth/2, windowHeight/2 + 20);
    
    run();
}

std::vector<std::pair<std::string, int>> LeaderboardWindow::loadLeaderboard() {

    std::vector<std::pair<std::string, int>> leaderboard;
    std::string line;
    std::ifstream ifs("../files/leaderboard.txt");
    
    while(std::getline(ifs, line)) {

        if(line.empty()) continue;
        size_t commaPos = line.find(',');
        if(commaPos == std::string::npos) continue;
        
        std::string left = line.substr(0, commaPos);
        std::string right = line.substr(commaPos+1);
        std::string name;
        int t = 0;
        
        if(left.find(':') != std::string::npos) {

            int mm=0, ss=0; char colon;
            std::istringstream ts(left);
            if(ts >> mm >> colon >> ss) t = mm*60 + ss;

            name = right;
        } 
        
        else if(right.find(':') != std::string::npos) {

            int mm=0, ss=0; char colon;
            std::istringstream ts(right);
            if(ts >> mm >> colon >> ss) t = mm*60 + ss;
            name = left;
        } 
        
        else {
            std::istringstream ts(right);
            if(ts >> t) name = left;

            else {

                std::istringstream ts2(left);
                if(ts2 >> t) name = right;
                else {
                    name = left;
                    t = 0;
                }
            }
        }

        leaderboard.push_back({name, t});
    }

    ifs.close();
    
    return leaderboard;
}

std::string LeaderboardWindow::formatLeaderboardContent(
    const std::vector<std::pair<std::string, int>>& leaderboard,
    const std::string& lastInsertedName, int lastInsertedTime, bool playerJustWon) {
    
    std::string leaderboardContent;
    bool marked = false;
    
    for(size_t i = 0; i < leaderboard.size(); ++i) {

        std::string name = leaderboard[i].first;
        int t = leaderboard[i].second;
        
        if(playerJustWon && !marked && name == lastInsertedName && t == lastInsertedTime) {

            name += "*";
            marked = true;
        }
        
        int mm = t / 60;
        int ss = t % 60;

        std::ostringstream oss;
        oss << std::setw(2) << std::setfill('0') << mm << ':'
            << std::setw(2) << std::setfill('0') << ss;

        std::string timestr = oss.str();
        leaderboardContent += timestr + "\t" + name;

        if(i < leaderboard.size() - 1) leaderboardContent += "\n\n";
    }
    
    return leaderboardContent;
}

void LeaderboardWindow::run() {

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {

            if(event.type == sf::Event::Closed) {
                
                window.close();
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(leaderboardTitle);
        window.draw(leaderboardText);
        window.display();
    }
}
