#include "Board.h"

Board::Board() : m_width(DEFAULT_BOARD_SIZE), m_height(DEFAULT_BOARD_SIZE) ,m_cellHeight(0), m_cellWidth(0) {
    grid.resize(static_cast<size_t>(m_width) * static_cast<size_t>(m_height));
    m_boardState.resize(m_height, std::vector<char>(m_width, ' '));
}

Board::Board(int width, int height) : m_width(width), m_height(height), m_cellHeight(0), m_cellWidth(0) {
    grid.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
    m_boardState.resize(m_height, std::vector<char>(m_width, ' '));
}

size_t Board::getIndex(const int x, const int y) const {
    return static_cast<size_t>(y) * static_cast<size_t>(m_width) + static_cast<size_t>(x);
}

void Board::clearData() {
    grid.clear();
    m_boardState.clear();
    m_width = 0;
    m_height = 0;
}

void Board::initializeBoard(const int cols, const int rows, const int W, const int H) {

    m_width = cols;
    m_height = rows;
    grid.clear();
    grid.resize(static_cast<size_t>(m_width) * static_cast<size_t>(m_height));
    m_boardState.clear();  // Clear old state
    m_boardState.resize(m_height, std::vector<char>(m_width, ' ')); // Default value

    m_cellWidth = static_cast<float>(W) / static_cast<float>(cols);
    m_cellHeight = (static_cast<float>(H) - TOOLBAR_HEIGHT) / static_cast<float>(rows);

    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            size_t index = getIndex(col, row);
            grid[index].setSize(sf::Vector2f(m_cellWidth, m_cellHeight));
            grid[index].setPosition(col * m_cellWidth, row * m_cellHeight + TOOLBAR_HEIGHT);
            grid[index].setFillColor(sf::Color::White);
            grid[index].setOutlineColor(sf::Color::Black);
            grid[index].setOutlineThickness(1.f);
        }
    }
}

void Board::handleMouseClick(const int mouseX, const int mouseY, const int selectedObject) {

    // Adjust mouseY to account for the toolbar
    int adjustedMouseY = mouseY - static_cast<int>(TOOLBAR_HEIGHT);

    int col = static_cast<int>(mouseX / m_cellWidth); 
    int row = static_cast<int>(adjustedMouseY / m_cellHeight); 

    if (col >= 0 && col < m_width && row >= 0 && row < m_height) {
        size_t index = getIndex(col, row);

        if (selectedObject == Object::DELETE) {
            grid[index].setFillColor(sf::Color::White); 
            grid[index].setTexture(nullptr); 
            m_boardState[row][col] = ' '; 
        }

        else if (selectedObject <= 5) {
            
            const sf::Texture& texture = getTextureForObject(selectedObject);
            if (&texture) {

                if (m_objectOrder[selectedObject] == "ROBOT") {
                    updateLocationRobot();
                }
                else if (m_objectOrder[selectedObject] == "DOOR") {
                    
                    if (row != 0 && col != 0 && row != m_boardState.size() - 1 && col != m_boardState[row].size() - 1) {
                        return;
                    }
                    updateLocationDoor();
                }

                grid[index].setTexture(&texture); 
                if (m_objectOrder[selectedObject] == "ROBOT") {
                    m_boardState[row][col] = '/'; 
                }
                else if (m_objectOrder[selectedObject] == "GUARD") {
                    m_boardState[row][col] = '!'; 
                }
                else if (m_objectOrder[selectedObject] == "WALL") {
                    m_boardState[row][col] = '#'; 
                }
                else if (m_objectOrder[selectedObject] == "ROCK") {
                    m_boardState[row][col] = '@'; 
                }
                else if (m_objectOrder[selectedObject] == "DOOR") {
                    m_boardState[row][col] = 'D'; 
                }
                else {
                    std::cerr << "Invalid object selected.\n";
                }

            }
            else {
                grid[index].setTexture(nullptr); 
                grid[index].setTexture(nullptr); 
                m_boardState[row][col] = ' '; 
            }
        }

    }
    else {
        std::cerr << "Click out of bounds! Column: " << col << ", Row: " << row << "\n";
    }
}

void Board::updateLocationRobot() {
    for (size_t row = 0; row < m_boardState.size(); ++row) { 
        for (size_t col = 0; col < m_boardState[row].size(); ++col) {  
            if (m_boardState[row][col] == '/') {
                m_boardState[row][col] = ' ';
                size_t index = getIndex(static_cast<int>(col), static_cast<int>(row));
                grid[index].setFillColor(sf::Color::White); 
                grid[index].setTexture(nullptr);
            }
        }
    }
}

void Board::updateLocationDoor() {
    for (size_t row = 0; row < m_boardState.size(); ++row) {  
        for (size_t col = 0; col < m_boardState[row].size(); ++col) { 
            if (m_boardState[row][col] == 'D') {
                m_boardState[row][col] = ' ';  
                size_t index = getIndex(static_cast<int>(col), static_cast<int>(row));  
                grid[index].setFillColor(sf::Color::White);  
                grid[index].setTexture(nullptr); 
            }
        }
    }
}

void Board::highlightCell(const int mouseX, const int mouseY, const  int H, const int W)
{
    static size_t lastHighlightedIndex = std::numeric_limits<size_t>::max(); 
    const float cellWidth = static_cast<float>(W) / m_width;  
    const float cellHeight = ((static_cast<float>(H)) - TOOLBAR_HEIGHT) / m_height;  
    const float adjustHeight = mouseY - TOOLBAR_HEIGHT;

    int col = static_cast<int>(mouseX / cellWidth);
    int row = static_cast<int>(adjustHeight / cellHeight);

    // Check if the mouse is within grid bounds
    if (col >= 0 && col < m_width && row >= 0 && row < m_height) {
        size_t currentIndex = getIndex(col, row);

        if (lastHighlightedIndex != currentIndex) {
            if (lastHighlightedIndex != std::numeric_limits<size_t>::max()) {
                grid[lastHighlightedIndex].setFillColor(sf::Color::White); 
            }

            grid[currentIndex].setFillColor(sf::Color(200, 200, 200)); 

            lastHighlightedIndex = currentIndex;  // Update last highlighted index
        }
    }
    else {
        if (lastHighlightedIndex != std::numeric_limits<size_t>::max()) {
            grid[lastHighlightedIndex].setFillColor(sf::Color::White); // Reset to default
            lastHighlightedIndex = std::numeric_limits<size_t>::max(); // Reset tracker
        }
    }



}


void Board::draw(sf::RenderWindow& window) {
    for (const auto& cell : grid) {
        window.draw(cell);
    }
}


bool Board::countRowsAndCols() {
    std::ifstream file(BOARD_FILE);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << BOARD_FILE << std::endl;
        return false;
    }

    std::string line;
    m_height = 0;
    m_width = 0;

    while (std::getline(file, line)) {
        m_height++; 

        if (m_height == 1) {
            m_width = static_cast<int>(line.length());
        }
    }
    file.close();
    return true;
}

bool Board::loadFromFile(int windowWidth, int windowHeight) {
    std::ifstream file(BOARD_FILE);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << BOARD_FILE << "\n";
        return false;
    }

    if (!countRowsAndCols()) {
        file.close();
        return false;
    }

    initializeBoard(m_width, m_height, windowWidth, windowHeight);

    // Map symbols to object names (e.g., '/' -> "ROBOT")
    std::map<char, std::string> symbolToName = {
        {'/', "ROBOT"},
        {'!', "GUARD"},
        {'#', "WALL"},
        {'@', "ROCK"},
        {'D', "DOOR"}
    };

    // Populate the board based on the file content
    for (int row = 0; row < m_height; ++row) {
        std::string line;
        if (!std::getline(file, line) || line.length() != static_cast<size_t>(m_width)) {
            std::cerr << "Error: Inconsistent or missing data in row " << row << " of the file.\n";
            file.close();
            return false;
        }

        for (int col = 0; col < m_width; ++col) {
            char symbol = line[col];
            m_boardState[row][col] = symbol; // Update internal board state
            size_t index = getIndex(col, row);

            if (symbol == ' ') {
                grid[index].setFillColor(sf::Color::White);
                grid[index].setTexture(nullptr);
                continue;
            }

            // Get the object name for the symbol
            auto it = symbolToName.find(symbol);
            if (it == symbolToName.end()) {
                std::cerr << "Unknown symbol '" << symbol << "' at row " << row << ", col " << col << "\n";
                continue;
            }

            const std::string& objectName = it->second;

            // Find the texture index from toolbarConfig
            auto toolbarIt = std::find(m_objectOrder.begin(), m_objectOrder.end(), objectName);
            if (toolbarIt == m_objectOrder.end()) {
                std::cerr << "Object name '" << objectName << "' not found in toolbarConfig.\n";
                grid[index].setTexture(nullptr);
                continue;
            }

            int textureIndex = std::distance(m_objectOrder.begin(), toolbarIt);

            // Safely apply the texture
            if (textureIndex >= 0 && textureIndex < static_cast<int>(m_textures.size())) {
                grid[index].setTexture(&m_textures[textureIndex]);
            }
            else {
                std::cerr << "Invalid texture index: " << textureIndex << " for object '" << objectName << "'\n";
                grid[index].setTexture(nullptr);
            }
        }
    }

    file.close();
    return true;
}



void Board::initializeTextures(std::string name) {
    m_objectOrder.push_back(name); // Keep track of the object name
    sf::Texture currentTexture;
    std::string filepath = name + ".png";

    if (currentTexture.loadFromFile(filepath)) {
        m_textures.push_back(currentTexture); // Store the texture
    }
    else {
        std::cerr << "Failed to load texture: " << filepath << std::endl;
    }
}


const sf::Texture& Board::getTextureForObject(const int& selectedObject) const {
    if (selectedObject < 0 || selectedObject >= m_textures.size()) {
        std::cerr << "Invalid object index: " << selectedObject << std::endl;
        return m_textures[Object::EMPTY]; // Return a default/empty texture
    }
    return m_textures[selectedObject];
}


bool Board::CheckExistFile() {
    std::ifstream checkFile(BOARD_FILE);
    return checkFile.good();
}

int Board::getRows() const {
    return m_height;
}

int Board::getCols() const {
    return m_width;
}


bool Board::saveToFile() const {
    for (const auto& row : m_boardState) {
        for (const auto& cell : row) {
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }

    std::ofstream outFile(BOARD_FILE);

    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << BOARD_FILE << " for writing.\n";
        return false;
    }

    for (const auto& row : m_boardState) {
        for (const auto& cell : row) {
            outFile << cell; // Write each cell to the file
        }
        outFile << '\n'; // New line after each row
    }

    outFile.close();
    if (!outFile) {
        std::cerr << "Error occurred while writing to file: " << BOARD_FILE << "\n";
        return false;
    }
    return true;
}
