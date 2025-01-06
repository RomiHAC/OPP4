#include "Board.h"

Board::Board() : m_width(10), m_height(10) {
    //    initializeTextures();
    grid.resize(static_cast<size_t>(m_width) * static_cast<size_t>(m_height));
    // Initialize m_boardState with default values (Object::DELETE)
    m_boardState.resize(m_height, std::vector<char>(m_width, ' '));

}

Board::Board(int width, int height) : m_width(width), m_height(height) {
    //   initializeTextures();
    grid.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
    // Initialize m_boardState with default values (Object::DELETE)
    m_boardState.resize(m_height, std::vector<char>(m_width, ' '));
}

size_t Board::getIndex(int x, int y) const {
    return static_cast<size_t>(y) * static_cast<size_t>(m_width) + static_cast<size_t>(x);
}

void Board::clearData() {
    grid.clear();
    m_boardState.clear();
    m_width = 0;
    m_height = 0;
}

void Board::initializeBoard(int cols, int rows, int W, int H) {
    //m_toolbarHeight = TOOLBAR_HEIGHT;
    m_width = cols;
    m_height = rows;
    std::cout << "m_width: " << m_width << ", m_height: " << m_height << "\n";
    // Clear and resize grid
    grid.clear();
    grid.resize(static_cast<size_t>(m_width) * static_cast<size_t>(m_height));

    // Initialize m_boardState
    m_boardState.clear();  // Clear old state
    m_boardState.resize(m_height, std::vector<char>(m_width, ' ')); // Default value

    float cellW = static_cast<float>(W) / static_cast<float>(cols);
    float cellH = (static_cast<float>(H) - TOOLBAR_HEIGHT) / static_cast<float>(rows);

    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            size_t index = getIndex(col, row);
            grid[index].setSize(sf::Vector2f(cellW, cellH));
            grid[index].setPosition(col * cellW, row * cellH + TOOLBAR_HEIGHT);
            grid[index].setFillColor(sf::Color::White);
            grid[index].setOutlineColor(sf::Color::Black);
            grid[index].setOutlineThickness(1.f);
        }
    }
}

//need to change according to int
void Board::handleMouseClick(int mouseX, int mouseY, int selectedObject) {
    float cellWidth = grid[0].getSize().x; // Cell width
    float cellHeight = grid[0].getSize().y; // Cell height

    // Adjust mouseY to account for the toolbar
    int adjustedMouseY = mouseY - TOOLBAR_HEIGHT;

    int col = static_cast<int>(mouseX / cellWidth); // Calculate the column
    int row = static_cast<int>(adjustedMouseY / cellHeight); // Calculate the row
    // std::cout << "selectedObject: " << selectedObject << ", Object::SAVE: " << Object::SAVE << "\n";

    if (col >= 0 && col < m_width && row >= 0 && row < m_height) {
        size_t index = getIndex(col, row);

        if (selectedObject == Object::DELETE) {
            grid[index].setFillColor(sf::Color::White); // Empty cell
            grid[index].setTexture(nullptr); // CLEAR texture
            m_boardState[row][col] = ' '; // Update board state to empty
        }

        else if (0 <= selectedObject <= 5) {
            // Load the texture associated with the selected object
            const sf::Texture& texture = getTextureForObject(selectedObject);
            if (&texture) {

                if (m_objectOrder[selectedObject] == "ROBOT") {
                    updateLocationRobot();
                }
                else if (m_objectOrder[selectedObject] == "DOOR") {
                    updateLocationDoor();
                }
                // std::cout << texture;
                grid[index].setTexture(&texture); // Set the new texture
                if (m_objectOrder[selectedObject] == "ROBOT") {
                    m_boardState[row][col] = '/'; // Robot
                }
                else if (m_objectOrder[selectedObject] == "GUARD") {
                    m_boardState[row][col] = '!'; // Guard
                }
                else if (m_objectOrder[selectedObject] == "WALL") {
                    m_boardState[row][col] = '#'; // Wall
                }
                else if (m_objectOrder[selectedObject] == "ROCK") {
                    m_boardState[row][col] = '@'; // Rock
                }
                else if (m_objectOrder[selectedObject] == "DOOR") {
                    m_boardState[row][col] = 'D'; // Door
                }
                else {
                    std::cerr << "Invalid object selected.\n";
                }

            }
            else {
                grid[index].setTexture(nullptr); // Clear any existing texture
                std::cerr << "Error: Texture not found for the selected object.\n";
                m_boardState[row][col] = ' '; // Update board state to empty
            }
        }

    }
    else {
        std::cerr << "Click out of bounds! Column: " << col << ", Row: " << row << "\n";
    }
}

void Board::updateLocationRobot() {
    for (size_t row = 0; row < m_boardState.size(); ++row) {  // Loop through rows
        for (size_t col = 0; col < m_boardState[row].size(); ++col) {  // Loop through columns
            if (m_boardState[row][col] == '/') {
                m_boardState[row][col] = ' ';
                size_t index = getIndex(static_cast<int>(col), static_cast<int>(row));
                grid[index].setFillColor(sf::Color::White); // Empty cell
                grid[index].setTexture(nullptr);
            }
        }
    }
}

void Board::updateLocationDoor() {
    for (size_t row = 0; row < m_boardState.size(); ++row) {  // Loop through rows
        for (size_t col = 0; col < m_boardState[row].size(); ++col) {  // Loop through columns
            if (m_boardState[row][col] == 'D') {
                m_boardState[row][col] = ' ';  // Clear the door ('D') cell
                size_t index = getIndex(static_cast<int>(col), static_cast<int>(row));  // Get grid index
                grid[index].setFillColor(sf::Color::White);  // Set to empty color
                grid[index].setTexture(nullptr);  // Remove door texture
            }
        }
    }
}




/// <summary>
/// added
/// </summary>
/// <param name="mouseX"></param>
/// <param name="mouseY"></param>
/// <param name="H"></param>
/// <param name="W"></param>
void Board::highlightCell(const int mouseX, const int mouseY, const  int H, const int W)
{
    static size_t lastHighlightedIndex = std::numeric_limits<size_t>::max(); // Track last highlighted cell
    // Consistent toolbar height
    const float cellWidth = static_cast<float>(W) / m_width;  // Dynamically calculate cell width
    const float cellHeight = ((static_cast<float>(H)) - TOOLBAR_HEIGHT) / m_height;  // Dynamic cell height 
    const float adjustHeight = mouseY - TOOLBAR_HEIGHT;

    int col = static_cast<int>(mouseX / cellWidth);
    int row = static_cast<int>(adjustHeight / cellHeight);

    //std::cout << "Mouse Position: (" << mouseX << ", " << mouseY << ")" << std::endl;
    //std::cout << "Adjusted MouseY: " << adjustHeight << std::endl;
    //std::cout << "Cell Dimensions: (" << cellWidth << ", " << cellHeight << ")" << std::endl;
    //std::cout << "Target Cell: Column " << col << ", Row " << row << std::endl;

    // Check if the mouse is within grid bounds
    if (col >= 0 && col < m_width && row >= 0 && row < m_height) {
        size_t currentIndex = getIndex(col, row);

        if (lastHighlightedIndex != currentIndex) {
            if (lastHighlightedIndex != std::numeric_limits<size_t>::max()) {
                grid[lastHighlightedIndex].setFillColor(sf::Color::White); // Reset to default
                //  std::cout << "Reset cell at index " << lastHighlightedIndex << " to white." << std::endl;
            }

            // Highlight the new cell
            grid[currentIndex].setFillColor(sf::Color(200, 200, 200)); // Gray highlight
            //
            //  std::cout << "Highlighted cell at index " << currentIndex << "." << std::endl;

            lastHighlightedIndex = currentIndex;  // Update last highlighted index
        }
    }
    else {
        if (lastHighlightedIndex != std::numeric_limits<size_t>::max()) {
            grid[lastHighlightedIndex].setFillColor(sf::Color::White); // Reset to default
            std::cout << "Mouse out of bounds. Reset cell at index " << lastHighlightedIndex << " to white." << std::endl;

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

    // Read the file line by line
    while (std::getline(file, line)) {
        m_height++;  // Increment row count

        if (m_height == 1) {
            // The first row defines the number of columns
            m_width = line.length();
        }
        //else if (line.length() != cols) {
        //    // Handle inconsistent column lengths
        //    std::cerr << "Warning: Inconsistent number of columns in row " << rows << std::endl;
        //}
    }
    std::cout << "Rows: " << m_height << ", Cols: " << m_width << "\n";
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
                std::cout << "Applied texture for '" << objectName << "' at row " << row << ", col " << col << std::endl;
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
        std::cout << "Successfully loaded texture: " << filepath << std::endl;
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

    std::cout << "Board saved successfully to " << BOARD_FILE << ".\n";
    return true;
}
