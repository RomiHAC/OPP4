#include "Toolbar.h"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
const int CONTROL_BUTTONS = 2; //save and clear buttons
Toolbar::Toolbar()
    : m_toolbarWidth(0), m_toolbarHeight(0), m_customCursor(nullptr) {
}

Toolbar::Toolbar(const std::string& configFile, const int windowWidth, const float cellHeight)
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
    toolbarConfig.push_back("DELETE");
    toolbarConfig.push_back("SAVE");
    toolbarConfig.push_back("CLEAR");

    return true;
}

std::vector<sf::Texture> Toolbar::loadTextures() const {
    std::vector<sf::Texture> textures;

    for (const auto& name : toolbarConfig) {
        sf::Texture texture;
        std::string filepath = name + ".png";

        if (texture.loadFromFile(filepath)) {
            textures.push_back(std::move(texture));
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

void Toolbar::createToolbar(const int windowWidth, const float cellHeight) {
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

int Toolbar::handleToolbarClick(int mouseX, sf::RenderWindow& window) {
    int buttonIndex = mouseX / (m_toolbarWidth / buttons.size());

    if (buttonIndex >= 0 && buttonIndex < (toolbarTextures.size() - CONTROL_BUTTONS)) {
        unsigned int buttonWidth = m_toolbarWidth / buttons.size();
        unsigned int buttonHeight = m_toolbarHeight;

        for (size_t i = 0; i < buttons.size(); ++i) {
            if (i == buttonIndex) {
                buttons[i].setFillColor(sf::Color(230, 230, 230)); // Highlight clicked button
            }
            else {
                buttons[i].setFillColor(sf::Color(100, 100, 100)); // Dim other buttons
            }
        }

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
        }
        else {
            std::cerr << "Failed to create custom cursor for button: " << toolbarConfig[buttonIndex] << std::endl;
        }
    }
    return buttonIndex;
}

const std::vector<std::string>& Toolbar::getToolbarConfig() const {
    return toolbarConfig;
}

void Toolbar::unDimButtons()
{
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].setFillColor(sf::Color(230, 230, 230)); // undim all buttons   
    }
}
