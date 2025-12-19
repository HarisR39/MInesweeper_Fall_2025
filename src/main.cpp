#include <SFML/Graphics.hpp>
#include "tile.hpp"
#include "Button.hpp"
#include "board.hpp"
#include "utils.hpp"
#include "WelcomeWindow.hpp"
#include "LeaderboardWindow.hpp"
#include "GameWindow.hpp"

int main() {
    board b;
    b.loadConfig("../files/config.cfg");
    b.numberLogic();

    int windowWidth = b.cols * 32;
    int windowHeight = b.rows * 32 + 100;

    sf::Font font;
    font.loadFromFile("../files/font.ttf");

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper", sf::Style::Close);
    window.setKeyRepeatEnabled(false);

    WelcomeWindow welcomeWindow(windowWidth, windowHeight, font);
    LeaderboardWindow leaderboardWindow(windowWidth, windowHeight, font);
    GameWindow* gameWindow = nullptr;

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {

                window.close();
            }

            if (welcomeWindow.isActive()) {

                welcomeWindow.handleEvent(event);
                
                if (!welcomeWindow.isActive()) {

                    std::string playerName = welcomeWindow.getName();
                    if(playerName.empty()) playerName = "Anon";
                    gameWindow = new GameWindow(windowWidth, windowHeight, playerName, font);
                }
            } else if (gameWindow) {
                gameWindow->handleEvent(event);
                
                if (event.type == sf::Event::MouseButtonPressed &&
                    event.mouseButton.button == sf::Mouse::Left) {

                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    
                    if (mouseY >= b.rows * 32 && gameWindow->shouldShowLeaderboard(mouseX, mouseY)) {

                        leaderboardWindow.show(
                            gameWindow->getLastInsertedName(),
                            gameWindow->getLastInsertedTime(),
                            gameWindow->getPlayerJustWon(),
                            window.getPosition().x,
                            window.getPosition().y
                        );
                    }
                }
            }
        }

        if (welcomeWindow.isActive()) {

            welcomeWindow.draw(window);
            window.display();
        } 

        else if (gameWindow) {

            gameWindow->update();
            gameWindow->draw(window);
            window.display();
        }
    }

    if (gameWindow) {
        
        delete gameWindow;
    }

    return 0;
}
