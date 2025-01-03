#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.h"

class WindowManager {
public:
    WindowManager();

   
    void displayWindow();        // Display and handle the main event loop
    void createToolbar();        // Create and configure the toolbar
    void handleToolbarClick(int mouseX); // Handl#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.h"
#include "Toolbar.h"

class WindowManager {
public:
    WindowManager();
    void displayWindow();        // Display and handle the main event loop
    float getCellWidth() const;  // Accessor for cell width
    float getCellHeight() const; // Accessor for cell height

private:
    Board m_board;
    Toolbar m_toolbar;
    int m_windowWidth;                 // Default window width
    int m_windowHeight;                 // Default window height
    std::vector<std::string> m_objectOrder;
    float m_cellWidth;       // Calculated cell width
    float m_cellHeight;      // Calculated cell height
    void setupWindow();          // Setup window dimensions and properties
    sf::RenderWindow m_window;   // SFML render window
    int m_currentToolIndex;
    char m_currentToolChar;
   // Object m_currentTool = Object::ROBOT; // Default tool
};
e clicks on the toolbar

    float getCellWidth() const;  // Accessor for cell width
    float getCellHeight() const; // Accessor for cell height
    /// <summary>
    /// shnage
    /// </summary>
    /// <returns></returns>
    sf::RenderWindow& getRenderWindow();
        
    
private:
    Board board;
    int W = 800;                 // Default window width
    int H = 600;                 // Default window height
    float cellWidth = 0.f;       // Calculated cell width
    float cellHeight = 0.f;      // Calculated cell height
    void setupWindow();          // Setup window dimensions and properties

    sf::RenderWindow m_window;   // SFML render window
    std::vector<sf::RectangleShape> toolbar; // Toolbar buttons
    std::vector<sf::Texture> toolbarTextures; // Toolbar button textures
    Object currentTool = Object::ROBOT; // Default tool
};
