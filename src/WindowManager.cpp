#include "WindowManager.h"
#include <iostream>

WindowManager::WindowManager() : m_windowWidth(1200), m_windowHeight(1000), m_cellWidth(0.f), m_cellHeight(0.f), m_currentToolIndex(-1),
    m_clickONtoolbar(false), m_ONwindow(false) {
    Toolbar gameToolbar("Toolbar.txt", m_windowWidth, TOOLBAR_HEIGHT);
    m_toolbar = gameToolbar;
    m_objectOrder = m_toolbar.getToolbarConfig();
    for (const auto& name : m_objectOrder) {
        m_board.initializeTextures(name);
    }
    setupWindow();
    displayWindow();
}

/// <summary>
/// apdate
/// </summary>
void WindowManager::setupWindow() {
    const int minWidth = 1200;          // Minimum window width
    const int minHeight = 1000;         // Minimum window height
    const int cellSize = 32;           // Size of each board cell

    if (m_board.CheckExistFile()) {
        m_windowWidth = std::max(minWidth, m_board.getCols() * cellSize);
        m_windowHeight = std::max(minHeight, static_cast<int>(TOOLBAR_HEIGHT) + m_board.getRows() * cellSize);
        m_board.loadFromFile(m_windowWidth, m_windowHeight);
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

    // Calculate cell dimensions
    m_cellWidth = (static_cast<float>(m_windowWidth)) / m_board.getCols();
    m_cellHeight = ((static_cast<float>(m_windowHeight)) - TOOLBAR_HEIGHT) / m_board.getRows();

    // Create the window with a fixed size
    m_window.create(sf::VideoMode(m_windowWidth, m_windowHeight), "Board Editor", sf::Style::Titlebar | sf::Style::Close);

    if (!m_window.isOpen()) {
        std::cerr << "Error: Failed to create window." << std::endl;
        return;
    }
}


/// <summary>
/// added now 
/// </summary>
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
                if (event.mouseButton.y > TOOLBAR_HEIGHT ) {  // Check mouse position relative to toolbar
                    if ( m_currentToolIndex >= 0) {
                        m_board.handleMouseClick(event.mouseButton.x, event.mouseButton.y, m_currentToolIndex);
                    }
                  
                    
                    
                }
             
                else {  // Toolbar area
                       updateONwindow(false);
                       updateclickONtoolbar(true);
                       updateCurrToolIndex(m_toolbar.handleToolbarClick(event.mouseButton.x, m_window));
                      // m_currentToolIndex = m_toolbar.handleToolbarClick(event.mouseButton.x, m_window);

                    if (m_currentToolIndex == Object::SAVE) {
                        if (m_board.saveToFile()) {
                            std::cout << "Board successfully saved to Board.txt.\n";
                        }
                        else {
                            std::cerr << "Failed to save the board to Board.txt.\n";
                        }
                        std::cout << "entered here: " << m_currentToolIndex << "\n";
                    }

                    if (m_currentToolIndex == Object::CLEAR) {
                        // Close the window
                        m_window.close();

                        // Clear the board data
                        m_board.clearData();

                        // Prompt the user for new dimensions
                        int width, height;
                        std::cout << "Enter new board dimensions:\n";
                        do {
                            std::cout << "Width (positive integer): ";
                            std::cin >> width;
                        } while (width <= 0);
                        do {
                            std::cout << "Height (positive integer): ";
                            std::cin >> height;
                        } while (height <= 0);

                        // Reinitialize the board with new dimensions
                        m_windowWidth = std::max(1200, width * 32); // Minimum width logic
                        m_windowHeight = std::max(1000, static_cast<int>(TOOLBAR_HEIGHT) + height * 32); // Minimum height logic
                        m_board.initializeBoard(width, height, m_windowWidth, m_windowHeight);

                        // Reopen the window
                        m_window.create(sf::VideoMode(m_windowWidth, m_windowHeight), "Board Editor", sf::Style::Titlebar | sf::Style::Close);
                        if (!m_window.isOpen()) {
                            std::cerr << "Error: Failed to recreate window.\n";
                            return;
                        }
                    }

                    std::cout << "Selected tool is in toolbar: " << m_currentToolIndex << std::endl;
                }
                break;

            case sf::Event::MouseMoved:

               
                if (event.mouseMove.y > TOOLBAR_HEIGHT) {  // Ensure mouse movement is below the toolbar
                    updateONwindow(true);
                   
                    if (!m_clickONtoolbar) {
                        changeMouse(event.mouseMove.y > TOOLBAR_HEIGHT);
                        
                    }
                   
                    m_board.highlightCell(event.mouseMove.x, event.mouseMove.y, m_windowHeight, m_windowWidth);
                }
                else {
                    
                    if (m_ONwindow) {
                        if (!m_clickONtoolbar) {
                            changeMouse(event.mouseMove.y > TOOLBAR_HEIGHT);
                            updateCurrToolIndex(-1);
                        }
                        updateclickONtoolbar(false);
                      
                    }



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


/// <summary>
/// update
/// </summary>
/// <param name="status"></param>
/// 

void WindowManager::updateCurrToolIndex(int tocheck)
{
    m_currentToolIndex = tocheck;
}
void WindowManager::updateclickONtoolbar(bool status) {
    m_clickONtoolbar = status;
}
void WindowManager::updateONwindow(bool status) {
    m_ONwindow = status;
}

/// <summary>
/// 
/// </summary>
/// <param name="aboveToolbar"></param>
void WindowManager::changeMouse(bool aboveToolbar)
{

    if (aboveToolbar) {
        sf::Cursor defaultCursor;
        if (defaultCursor.loadFromSystem(sf::Cursor::Arrow)) {
            m_window.setMouseCursor(defaultCursor); // Reset cursor to default
        }
    }
    else {
        sf::Cursor customCursor;
        if (customCursor.loadFromSystem(sf::Cursor::Hand)) {

            m_window.setMouseCursor(customCursor); // Set custom cursor

        }
    }
}
