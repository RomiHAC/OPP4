#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
const float TOOLBAR_HEIGHT = 50.f; // Height of the toolbarHeight
const int NUMBER_OF_OBJECTS = 5;
enum Object {
    SAVE = NUMBER_OF_OBJECTS,
    DELETE,
    REMOVE,
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
    void handleMouseClick(int mouseX, int mouseY, char selectedObject);
    //added
    void highlightCell(int mouseX, int mouseY, int H, int W);
    //bool isRobotPresent();
    //
    void draw(sf::RenderWindow& window);
    sf::Texture* getTextureForObject(char selectedObject);
    void initializeTextures();


    int getRows() const;
    int getCols() const;

private:
    std::vector<std::vector<char>> m_boardState;
    std::vector<sf::Texture> m_objectTextures;
    int m_width;
    int m_height;
    //float m_toolbarHeight;
    float m_offsetX;
    float m_offsetY;
    std::vector<sf::RectangleShape> grid;
    const std::string BOARD_FILE = "Board.txt";
    size_t getIndex(int x, int y) const;
    bool countRowsAndCols();
    std::vector<sf::Texture> m_textures;
    std::vector<std::string> toolbarConfig;
    // Texture objects for different objects
    sf::Texture robotTexture;
    sf::Texture guardTexture;
    sf::Texture doorTexture;
    sf::Texture wallTexture;
    sf::Texture rockTexture;

};
