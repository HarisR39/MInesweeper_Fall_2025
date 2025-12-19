#ifndef WELCOMEWINDOW_HPP
#define WELCOMEWINDOW_HPP

#include <string>
#include <SFML/Graphics.hpp>

class WelcomeWindow {
public:
    WelcomeWindow(int windowWidth, int windowHeight, sf::Font& font);
    
    void handleEvent(const sf::Event& event);
    void draw(sf::RenderWindow& window);
    bool isActive() const { return active; }
    std::string getName() const { return name; }
    void deactivate() { active = false; }

private:
    bool active;
    std::string name;
    sf::Text welcomeText;
    sf::Text enterNameText;
    sf::Text nameText;
    int windowWidth;
    int windowHeight;
    sf::Font& font;
    
    void updateNameText();
};

#endif
