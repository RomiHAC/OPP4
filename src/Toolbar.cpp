#include "Toolbar.h"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
const int CONTROL_BUTTONS = 3;
Toolbar::Toolbar()
    : m_toolbarWidth(0), m_toolbarHeight(0), m_customCursor(nullptr) {
}

Toolbar::Toolbar(const std::string& configFile, int windowWidth, float cellHeight)
    : m_toolbarWidth(windowWidth), m_toolbarHeight(cellHeight), m_customCursor(nullptr) {
    if (loadConfig(configFile)) {
        createToolbar(windowWidth, cellHeight);  
    }
}

bool Toolbar::loadConfig(const std::string& configFile) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Cannot open toolbar configuration file: " << configFile << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        for (char c : line) {
            switch (c) {
            case '/': toolbarConfig.push_back("ROBOT"); break;
            case '!': toolbarConfig.push_back("GUARD"); break;
            case 'D': toolbarConfig.push_back("DOOR"); break;
            case '#': toolbarConfig.push_back("WALL"); break;
            case '@': toolbarConfig.push_back("ROCK"); break;
            default:
                std::cerr << "Unknown toolbar character: " << c << "\n";
                break;
            }
        }
    }

    file.close();
    toolbarConfig.push_back("SAVE");
    toolbarConfig.push_back("DELETE");
    toolbarConfig.push_back("REMOVE");

    return true;
}

std::vector<sf::Texture> Toolbar::loadTextures() {
    std::vector<sf::Texture> textures;

    std::cout << "Number of toolbar items: " << toolbarConfig.size() << std::endl;

    for (const auto& name : toolbarConfig) {
        sf::Texture texture;
        std::string filepath = name + ".png";  

        if (texture.loadFromFile(filepath)) {
            textures.push_back(std::move(texture));
            std::cout << "Loaded Texture: " << filepath << std::endl;
        }
        else {
            std::cerr << "Failed to load image: " << filepath << std::endl;
        }
    }

    return textures;
}

void Toolbar::draw(sf::RenderWindow& window) const {
    for (const auto& button : buttons) {
        window.draw(button); 
    }
}

void Toolbar::createToolbar(int windowWidth, float cellHeight) {
    toolbarTextures = loadTextures();

    float buttonWidth = (static_cast<float>(windowWidth)) / toolbarConfig.size();
    float buttonHeight = cellHeight;

    buttons.clear();


    for (int i = 0; i < toolbarConfig.size(); ++i) {
        sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
        button.setPosition(i * buttonWidth, 0);
        button.setTexture(&toolbarTextures[i]);
        button.setOutlineColor(sf::Color::White);
        button.setOutlineThickness(1);

        buttons.push_back(button);
    }
}

char Toolbar::handleToolbarClick(int mouseX, sf::RenderWindow& window) {
    char selectedObject = ' ';
    int buttonIndex = mouseX / (m_toolbarWidth / buttons.size());  
    if (buttonIndex >= 0 && buttonIndex < buttons.size()) {
        std::cout << "Clicked on toolbar button index: " << buttonIndex << std::endl;
        std::cout << "Button name: " << toolbarConfig[buttonIndex] << std::endl;

    }

    if (buttonIndex >= 0 && buttonIndex < (toolbarTextures.size())) {
        unsigned int buttonWidth = m_toolbarWidth / buttons.size();
        unsigned int buttonHeight = m_toolbarHeight;  


        if (m_customCursor) {
            delete m_customCursor; 
        }

        sf::Texture cursorTexture;
        cursorTexture.loadFromImage(toolbarTextures[buttonIndex].copyToImage());  

     
        sf::Image cursorImage = cursorTexture.copyToImage();
        sf::Image resizedImage;
        resizedImage.create(buttonWidth, buttonHeight, sf::Color::Transparent);  

        for (unsigned int y = 0; y < buttonHeight; ++y) {
            for (unsigned int x = 0; x < buttonWidth; ++x) {
                unsigned int srcX = x * cursorImage.getSize().x / buttonWidth;
                unsigned int srcY = y * cursorImage.getSize().y / buttonHeight;
                resizedImage.setPixel(x, y, cursorImage.getPixel(srcX, srcY));
            }
        }

        // Calculate the center of the resized image
        sf::Vector2u cursorSize = resizedImage.getSize();
        sf::Vector2u hotspot(cursorSize.x / 2, cursorSize.y / 2);
        m_customCursor = new sf::Cursor();
        if (m_customCursor->loadFromPixels(resizedImage.getPixelsPtr(), resizedImage.getSize(), hotspot)) {
            window.setMouseCursor(*m_customCursor);
            std::cout << "Cursor changed to custom for button: " << toolbarConfig[buttonIndex] << std::endl;
        }
        else {
            std::cerr << "Failed to create custom cursor for button: " << toolbarConfig[buttonIndex] << std::endl;
        }
    }

        if (toolbarConfig[buttonIndex] == "ROBOT") {
            selectedObject = '/';
        }
        else if (toolbarConfig[buttonIndex] == "GUARD") {
            selectedObject = '!';
        }
        else if (toolbarConfig[buttonIndex] == "DOOR") {
            selectedObject = 'D';
        }
        else if (toolbarConfig[buttonIndex] == "WALL") {
            selectedObject = '#';
        }
        else if (toolbarConfig[buttonIndex] == "ROCK") {
            selectedObject = '@';
        }
        else if (toolbarConfig[buttonIndex] == "SAVE") {
            selectedObject = 's';
        }
        else if (toolbarConfig[buttonIndex] == "DELETE") {
            selectedObject = 'd';
        }
        else if (toolbarConfig[buttonIndex] == "REMOVE") {
            selectedObject = 'r';
        }
    return selectedObject;
}

const std::vector<std::string>& Toolbar::getToolbarConfig() const {
    return toolbarConfig;
}
