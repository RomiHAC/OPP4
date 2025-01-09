#include <SFML/Graphics.hpp>
#include <vector>
#include "Board.h"
#include "Toolbar.h"
const int UN_TOUCHED = 9;
const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 1000;         // Minimum window height
const int CELL_SIZE = 32;           // Size of each board cell

class WindowManager {
public:
    WindowManager();
    void displayWindow();        // Display and handle the main event loop
    void updateclickONtoolbar(bool status);
    void updateONwindow(bool status);
    void  updateCurrToolIndex(int currentToolIndex);
    void setupWindow();             // Setup window dimensions and properties

private:
    Board m_board;
    Toolbar m_toolbar;
    int m_windowWidth;                 // Default window width
    int m_windowHeight;                 // Default window height
    std::vector<std::string> m_objectOrder;
    sf::RenderWindow m_window;   // SFML render window
    int m_currentToolIndex;

    bool m_OnWindow;
    bool m_ClickOnToolbar;

    void checkFileExist();          // Setup window dimensions and properties

    void changeMouse(bool aboveToolbar);

};
