#include "Draw.h"
#include <iostream>

WindowManager::WindowManager() {
    setupWindow();

    if(!board.loadToolbarConfig("Toolbar.txt")){}

    createToolbar();

    displayWindow();
}

/// <summary>
/// made a change
/// </summary>
void WindowManager::setupWindow() {
    const int minWidth = 800;          // Minimum window width
    const int minHeight = 600;         // Minimum window height
    const int cellSize = 32;           // Size of each board cell
    const int toolbarHeight = 50;      // Height of the toolbar

    if (board.CheckExistFile()) {
        board.loadFromFile();
        W = std::max(minWidth, board.getCols() * cellSize);
        H = std::max(minHeight, toolbarHeight + board.getRows() * cellSize);
    }
    else {
        int width, height;
        std::cout << "Board file not found. Please enter the board dimensions:\n";
        board.clearData();

        do {
            std::cout << "Width (positive integer): ";
            std::cin >> width;
        } while (width <= 0);

        do {
            std::cout << "Height (positive integer): ";
            std::cin >> height;
        } while (height <= 0);

        W = std::max(minWidth, width * cellSize);
        H = std::max(minHeight, toolbarHeight + height * cellSize);
        board.initializeBoard(width, height, W, H);
    }
     
    // Calculate window dimensions
    

    cellWidth = (static_cast<float>(W)) / board.getCols();
    cellHeight = ((static_cast<float>(H)) - toolbarHeight) / board.getRows();

    // Create the window
    m_window.create(sf::VideoMode(W, H), "Board Editor");
}


void WindowManager::displayWindow() {
    while (m_window.isOpen()) {
        m_window.clear();  // Clear the window

        // Draw the board
        board.draw(m_window);

        // Draw the toolbar
        for (const auto& button : toolbar) {
            m_window.draw(button);
        }

        m_window.display();  // Show everything we've drawn so far

        sf::Event event;
        while (m_window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                m_window.close();
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.y > 50) {  // Check mouse position relative to toolbar
                    board.handleMouseClick(event.mouseButton.x, event.mouseButton.y - 50, currentTool);
                }
                else {  // Toolbar area
                    handleToolbarClick(event.mouseButton.x);
                }
                break;

            case sf::Event::MouseMoved:
                   // Check mouse position relative to toolbar
                    board.highlightCell(event.mouseMove.x, event.mouseMove.y, H, W, &getRenderWindow());
                
                break;
            }
        }
    }
}



/// <summary>
/// made a change 
/// </summary>
void WindowManager::createToolbar() {
    toolbarTextures = board.loadTextures();  // Load textures from the Board class

    float buttonWidth = static_cast<float>(W) / 8;
    float buttonHeight = 50.0f;

    toolbar.clear();  // Clear previous toolbar buttons

    // Check if textures loaded correctly
    if (toolbarTextures.empty()) {
        std::cerr << "No textures loaded for toolbar." << std::endl;
        return;
    }

    // Create toolbar buttons
    for (size_t i = 0; i < board.getToolbarConfig().size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setFillColor(sf::Color::White);           // Set a default color for testing
        button.setPosition(i * buttonWidth, 0);          // Position buttons with spacing

        if (i < toolbarTextures.size()) {
            button.setTexture(&toolbarTextures[i]);      // Bind the texture
        }
        else {
            std::cerr << "Texture missing for button " << i << std::endl;
        }

        button.setOutlineColor(sf::Color::White);
        button.setOutlineThickness(1);

        toolbar.push_back(button);  // Add button to the toolbar vector
    }
}





void WindowManager::handleToolbarClick(int mouseX) {
    int buttonIndex = mouseX / (W / toolbar.size());  // Calculate button index based on toolbar width
    if (buttonIndex >= 0 && buttonIndex < toolbar.size()) {
        currentTool = static_cast<Object>(buttonIndex);  // Assign selected tool from toolbar
    }
}

float WindowManager::getCellWidth() const
{
    return cellWidth;
}

float WindowManager::getCellHeight() const
{
    return cellHeight;
}
//change \
////
////
sf::RenderWindow& WindowManager::getRenderWindow() 
{
    return m_window;
}
