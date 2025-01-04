#include "WindowManager.h"
#include <iostream>

WindowManager::WindowManager() : m_windowWidth(800), m_windowHeight(600), m_cellWidth(0.f), m_cellHeight(0.f), m_currentToolIndex(0) {
    setupWindow();
    Toolbar gameToolbar("Toolbar.txt", m_windowWidth, TOOLBAR_HEIGHT);
    m_toolbar = gameToolbar;
    m_objectOrder = m_toolbar.getToolbarConfig();
    displayWindow();
}

void WindowManager::setupWindow() {
    const int minWidth = 1200;          // Minimum window width
    const int minHeight = 1000;         // Minimum window height
    const int cellSize = 32;           // Size of each board cell

    if (m_board.CheckExistFile()) {
    
        m_windowWidth = std::max(minWidth, m_board.getCols() * cellSize);
        m_windowHeight = std::max(minHeight, static_cast<int>(TOOLBAR_HEIGHT) + m_board.getRows() * cellSize);
        m_board.loadFromFile(m_windowWidth, m_windowWidth);
    }
    else {
        int width, height;
        std::cout << "Board file not found. Please enter the board dimensions:\n";
        m_board.clearData();

        do {
            std::cout << "Width (positive integer): ";
            std::cin >> width;
        } while (width <= 0);

        do {
            std::cout << "Height (positive integer): ";
            std::cin >> height;
        } while (height <= 0);

        m_windowWidth = std::max(minWidth, width * cellSize);
        m_windowHeight = std::max(minHeight, static_cast<int>(TOOLBAR_HEIGHT) + height * cellSize);
        m_board.initializeBoard(width, height, m_windowWidth, m_windowHeight);
    }

    // Validate window dimensions before creating the window
    if (m_windowWidth <= 0 || m_windowHeight <= 0) {
        std::cerr << "Error: Invalid window dimensions. Window creation failed." << std::endl;
        return;  // Exit if invalid dimensions
    }

    // Calculate cell dimensions
    m_cellWidth = (static_cast<float>(m_windowWidth)) / m_board.getCols();
    m_cellHeight = ((static_cast<float>(m_windowHeight)) - TOOLBAR_HEIGHT) / m_board.getRows();

    // Create the window with validated dimensions
    m_window.create(sf::VideoMode(m_windowWidth, m_windowHeight), "Board Editor");

    // Check if the window was created successfully
    if (!m_window.isOpen()) {
        std::cerr << "Error: Failed to create window." << std::endl;
        return;  // Exit if window creation failed
    }
}

void WindowManager::displayWindow() {
    while (m_window.isOpen()) {
        m_window.clear();
        m_board.draw(m_window);
        m_toolbar.draw(m_window);
        m_window.display();

        sf::Event event;
        while (m_window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                m_window.close();
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.y > TOOLBAR_HEIGHT) {  // Check mouse position relative to toolbar
                    m_board.handleMouseClick(event.mouseButton.x, event.mouseButton.y, m_currentToolChar);
                }
                else {  // Toolbar area
                    m_currentToolChar = m_toolbar.handleToolbarClick(event.mouseButton.x, m_window);

                    if (m_currentToolChar == 's') {
                        // Save the board to a file
                        if (m_board.saveToFile()) {
                            std::cout << "Board saved successfully!\n";
                        }
                        else {
                            std::cerr << "Failed to save the board.\n";
                        }   
                    }
                    if (m_currentToolChar == 'd') {
                        //logic
                    }
                    if (m_currentToolChar == 'r' ){
                        //logic
                    }
                    std::cout << "Selected tool is in toolbar: " << m_currentToolChar << std::endl;
                }
                break;

            case sf::Event::MouseMoved:
                if (event.mouseMove.y > TOOLBAR_HEIGHT) {  // Ensure mouse movement is below the toolbar
                    // board.highlightCell(event.mouseMove.x, event.mouseMove.y, m_windowHeight, m_W, m_cellHeight);
                }
                break;
            }
        }
    }
}

float WindowManager::getCellWidth() const {
    return m_cellWidth;
}

float WindowManager::getCellHeight() const {
    return m_cellHeight;
}
