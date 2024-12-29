#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

enum Object {
    ROBOT, GUARD, DOOR, WALL, ROCK, SAVE,
    DELETE, REMOVE
};

class Board {
public:
    Board();
    Board(int width, int height);

    void clearData();

    bool saveToFile() const;
    bool loadFromFile();
    bool CheckExistFile();

    bool loadToolbarConfig(const std::string& filepath);

    std::vector<sf::Texture> loadTextures();
    void initializeBoard(int cols, int rows, int W, int H);
    void handleMouseClick(int mouseX, int mouseY, Object selectedObject);
    void highlightCell(int mouseX, int mouseY, int H, int W);
    sf::Color getColorForObject(Object obj);
    bool isRobotPresent();

    void draw(sf::RenderWindow& window);

    int getRows() const { return height; }
    int getCols() const { return width; }
    const std::vector<std::string>& getToolbarConfig() const { return toolbarConfig; }


private:
    int width = 0;
    int height = 0;
    std::vector<sf::RectangleShape> grid;
    const std::string BOARD_FILE = "Board.txt";

    std::vector<std::string> toolbarConfig; // Store toolbar configuration
    size_t getIndex(int x, int y) const;

};
