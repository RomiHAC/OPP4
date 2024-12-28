#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.h"

class WindowManager {
public:
    WindowManager();

   
    void displayWindow();        // Display and handle the main event loop
    void createToolbar();        // Create and configure the toolbar
    void handleToolbarClick(int mouseX); // Handle clicks on the toolbar

    float getCellWidth() const;  // Accessor for cell width
    float getCellHeight() const; // Accessor for cell height

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
