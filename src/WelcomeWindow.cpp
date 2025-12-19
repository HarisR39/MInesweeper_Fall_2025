#include "WelcomeWindow.hpp"
#include "utils.hpp"
#include <cctype>

WelcomeWindow::WelcomeWindow(int windowWidth, int windowHeight, sf::Font& font)
    : active(true), name("|"), windowWidth(windowWidth), windowHeight(windowHeight), font(font) {
    
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    setText(welcomeText, windowWidth/2, windowHeight/2 - 150);

    enterNameText.setFont(font);
    enterNameText.setString("Enter your name:");
    enterNameText.setCharacterSize(20);
    enterNameText.setFillColor(sf::Color::White);
    setText(enterNameText, windowWidth/2, windowHeight/2 - 75);

    nameText.setFont(font);
    nameText.setCharacterSize(18);
    nameText.setFillColor(sf::Color::Yellow);
    nameText.setString(name);
    setText(nameText, windowWidth/2, windowHeight/2 - 45);
}

void WelcomeWindow::handleEvent(const sf::Event& event) {

    if (event.type == sf::Event::KeyPressed) {
        sf::Keyboard::Key key = event.key.code;

        if (key == sf::Keyboard::Backspace) {

            if (!(name == "|")) {
                name.pop_back();
                name.pop_back();
                name.push_back('|');
            }

        } else if (key == sf::Keyboard::Enter && name != "|") {

            name.pop_back();
            active = false;

        } 
        
        else {

            if (name.size() < 11 &&
                key >= sf::Keyboard::A && key <= sf::Keyboard::Z) {
                
                char letter = 'a' + (key - sf::Keyboard::A);

                if (name == "|")
                    letter = static_cast<char>(std::toupper(letter));
                else
                    letter = static_cast<char>(std::tolower(letter));

                name.pop_back();
                name.push_back(letter);
                name.push_back('|');
            }
        }
        
        updateNameText();
    }
}

void WelcomeWindow::draw(sf::RenderWindow& window) {

    window.clear(sf::Color::Blue);
    window.draw(welcomeText);
    window.draw(enterNameText);
    window.draw(nameText);
}

void WelcomeWindow::updateNameText() {
    
    nameText.setString(name);
    setText(nameText, windowWidth/2, windowHeight/2 - 45);
}
