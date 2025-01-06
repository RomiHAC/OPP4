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
    void loadTexture();



    //check
    const std::vector<std::string>& getToolbarConfig() const;
   
   

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
    // 
    // 
    /////added
    void changeMouse(bool aboveToolbar) ;
    bool clickONtoolbar = false;
   
};
