#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Toolbar {
public:
    Toolbar();
    Toolbar(const std::string& configFile, const int windowWidth, const float cellHeight);
    bool loadConfig(const std::string& configFile);
    void draw(sf::RenderWindow& window) const;
    std::vector<sf::Texture> loadTextures() const;
    void createToolbar(const int windowWidth, const float cellHeight);
    int handleToolbarClick(int mouseX, sf::RenderWindow& window);
    const std::vector<std::string>& getToolbarConfig() const;
    void unDimButtons();


private:

    std::vector<std::string> toolbarConfig;
    std::vector<sf::Texture> toolbarTextures;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::RectangleShape> toolbar;
    std::vector<std::string> objectTypes;

    int m_toolbarWidth;         // Width of the toolbar
    int m_toolbarHeight;        // Height of the toolbar

    // Custom cursor for when a button is clicked
    sf::Cursor* m_customCursor;  // Pointer to a custom cursor 

};
