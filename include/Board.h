#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
const float TOOLBAR_HEIGHT = 150.f; // Height of the toolbarHeight
const int NUMBER_OF_OBJECTS = 5;
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

    void initializeBoard(int cols, int rows, int W, int H);
    void handleMouseClick(int mouseX, int mouseY, int selectedObject);

    //added
    /// <summary>
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
    std::vector<sf::RectangleShape> grid;
    const std::string BOARD_FILE = "Board.txt";
    size_t getIndex(int x, int y) const;
    bool countRowsAndCols();
    std::vector<sf::Texture> m_textures;
    std::vector<std::string> toolbarConfig;
    std::vector<char> m_orderOfObjects;

};
