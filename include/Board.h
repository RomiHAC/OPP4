#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
const float TOOLBAR_HEIGHT = 150.f; // Height of the toolbarHeight
const int NUMBER_OF_OBJECTS = 5;
const int DEFAULT_BOARD_SIZE = 10;
const std::string BOARD_FILE = "Board.txt";

enum Object {
    DELETE = NUMBER_OF_OBJECTS,
    SAVE,
    CLEAR,
    EMPTY
};

class Board {
public:
    Board();
    Board(int width, int height);

    void clearData();
    bool saveToFile() const;
    bool loadFromFile(int windowWidth, int windowHeight);
    bool CheckExistFile();
    void initializeBoard(const int cols, const int rows, const int W, const int H);
    void handleMouseClick(const int mouseX, const int mouseY, const int selectedObject);
    void highlightCell(const int mouseX, const int mouseY, const int H, const int W);
    void updateLocationRobot();
    void updateLocationDoor();
    void draw(sf::RenderWindow& window);
    const sf::Texture& getTextureForObject(const int& selectedObject) const;
    void initializeTextures(std::string name);
    int getRows() const;
    int getCols() const;

private:
    std::vector<std::vector<char>> m_boardState;
    std::vector<std::string> m_objectOrder;

    int m_width;
    int m_height;
    float m_cellWidth;
    float m_cellHeight;
    std::vector<sf::RectangleShape> grid;
    std::vector<sf::Texture> m_textures;
    std::vector<std::string> toolbarConfig;

    size_t getIndex(int x, int y) const;
    bool countRowsAndCols();

};
