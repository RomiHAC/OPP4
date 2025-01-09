#include "WindowManager.h"
#include <iostream>


WindowManager::WindowManager() : m_windowWidth(WINDOW_WIDTH), m_windowHeight(WINDOW_HEIGHT), m_currentToolIndex(UN_TOUCHED),
m_ClickOnToolbar(false), m_OnWindow(false) {
    m_toolbar = Toolbar("Toolbar.txt", m_windowWidth, TOOLBAR_HEIGHT);
    m_objectOrder = m_toolbar.getToolbarConfig();
    for (const auto& name : m_objectOrder) {
        m_board.initializeTextures(name);
    }
    setupWindow();
    displayWindow();
}

void WindowManager::setupWindow() {

    if (m_board.CheckExistFile()) {
        m_windowWidth = std::max(m_windowWidth, m_board.getCols() * CELL_SIZE);
        m_windowHeight = std::max(m_windowHeight, static_cast<int>(TOOLBAR_HEIGHT) + m_board.getRows() * CELL_SIZE);
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

        m_windowWidth = std::max(m_windowWidth, width * CELL_SIZE);
        m_windowHeight = std::max(m_windowHeight, static_cast<int>(TOOLBAR_HEIGHT) + height * CELL_SIZE);
        m_board.initializeBoard(width, height, m_windowWidth, m_windowHeight);
    }

    // Create the window with a fixed size
    //can Add a title + photo
    m_window.create(sf::VideoMode(m_windowWidth, m_windowHeight), "Board Editor", sf::Style::Titlebar | sf::Style::Close);

    if (!m_window.isOpen()) {
        std::cerr << "Error: Failed to create window." << std::endl;
        return;
    }
}

void WindowManager::displayWindow() {
    while (m_window.isOpen()) {
        m_window.clear(sf::Color::Black);  // Set the background to black
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
                if (event.mouseButton.y > TOOLBAR_HEIGHT) {
                    if (m_currentToolIndex != UN_TOUCHED) {
                        m_board.handleMouseClick(event.mouseButton.x, event.mouseButton.y, m_currentToolIndex);
                    }
                }
                else {
                    updateONwindow(false);
                    updateclickONtoolbar(true);
                    updateCurrToolIndex(m_toolbar.handleToolbarClick(event.mouseButton.x, m_window));

                    if (m_currentToolIndex == Object::SAVE) {
                        if (m_board.saveToFile()) {
                            std::cout << "Board successfully saved to Board.txt.\n";
                        }
                        else {
                            std::cerr << "Failed to save the board to Board.txt.\n";
                        }
                    }

                    if (m_currentToolIndex == Object::CLEAR) {
                        m_window.close();
                        m_board.clearData();

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

                        m_windowWidth = std::max(WINDOW_WIDTH, width * CELL_SIZE);
                        m_windowHeight = std::max(WINDOW_HEIGHT, static_cast<int>(TOOLBAR_HEIGHT) + height * CELL_SIZE);
            
                        m_board.initializeBoard(width, height, m_windowWidth, m_windowHeight);

                        m_window.create(sf::VideoMode(m_windowWidth, m_windowHeight), "Board Editor", sf::Style::Titlebar | sf::Style::Close);
                        if (!m_window.isOpen()) {
                            std::cerr << "Error: Failed to recreate window.\n";
                            return;
                        }
                    }
                }
                break;

            case sf::Event::MouseMoved:
                if (event.mouseMove.y > TOOLBAR_HEIGHT) {
                    updateONwindow(true);
                    if (!m_ClickOnToolbar) {
                        changeMouse(event.mouseMove.y > TOOLBAR_HEIGHT);

                    }
                    m_board.highlightCell(event.mouseMove.x, event.mouseMove.y, m_windowHeight, m_windowWidth);
                }
                else {
                    if (m_OnWindow) {
                        m_toolbar.unDimButtons();
                        if (!m_ClickOnToolbar) {
                            changeMouse(event.mouseMove.y > TOOLBAR_HEIGHT);
                            updateCurrToolIndex(UN_TOUCHED);
                        }
                        updateclickONtoolbar(false);
                    }
                }
                break;
            }
        }
    }
}

void WindowManager::updateCurrToolIndex(int currentToolIndex)
{
    m_currentToolIndex = currentToolIndex;
}
void WindowManager::updateclickONtoolbar(bool status) {
    m_ClickOnToolbar = status;
}
void WindowManager::updateONwindow(bool status) {
    m_OnWindow = status;
}

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
