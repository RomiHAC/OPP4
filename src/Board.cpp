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
void Board::handleMouseClick(int mouseX, int mouseY, int selectedObject) {
    float cellWidth = grid[0].getSize().x; // Cell width
    float cellHeight = grid[0].getSize().y; // Cell height

    // Adjust mouseY to account for the toolbar
    int adjustedMouseY = mouseY - TOOLBAR_HEIGHT;

    int col = static_cast<int>(mouseX / cellWidth); // Calculate the column
    int row = static_cast<int>(adjustedMouseY / cellHeight); // Calculate the row

    if (col >= 0 && col < m_width && row >= 0 && row < m_height) {
        size_t index = getIndex(col, row);

        if (selectedObject == Object::REMOVE) {
            grid[index].setFillColor(sf::Color::White); // Empty cell
            grid[index].setTexture(nullptr); // Remove texture
            m_boardState[row][col] = ' '; // Update board state to empty
        }
       // else if(m_boardState[row][col] == ' ') {
       
            // Load the texture associated with the selected object
            sf::Texture texture = getTextureForObject(selectedObject);
            if (&texture) {

                if (selectedObject == '/') {
                    updateLocationRobot();
                }
               // std::cout << texture;
                grid[index].setTexture(&texture); // Set the new texture
                m_boardState[row][col] = selectedObject; // Update board state
            }
            else {
                grid[index].setTexture(nullptr); // Clear any existing texture
                std::cerr << "Error: Texture not found for the selected object.\n";
                m_boardState[row][col] = ' '; // Update board state to empty
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



/// <summary>
/// added
/// </summary>
/// <param name="mouseX"></param>
/// <param name="mouseY"></param>
/// <param name="H"></param>
/// <param name="W"></param>
void Board::highlightCell(const int mouseX, const int mouseY,const  int H,const int W)
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
    if (!countRowsAndCols()) {
        return false;
    }
    ;
    std::cout << "m_width: " << windowWidth << ", m_height: " << windowHeight << "\n";

    // Initialize board dimensions and m_boardState
    initializeBoard(m_width, m_height, windowWidth, windowHeight);

    for (int row = 0; row < m_height; ++row)
    {
        for (int col = 0; col < m_width; ++col)
        {
            char symbol;
            file >> symbol;
            m_boardState[row][col] = symbol;

            // Set the texture and color for the grid cell
            size_t index = getIndex(col, row);
            if (m_boardState[row][col] == ' ') {
                grid[index].setFillColor(sf::Color::White);
                grid[index].setTexture(nullptr);
            }
            else {
                sf::Texture texture = getTextureForObject(m_boardState[row][col]);
                if (&texture) {
                    grid[index].setTexture(nullptr); // Clear the existing texture
                    grid[index].setTexture(&texture); // Set the new texture
                }
                else {
                    std::cerr << "Error: Invalid texture for object type " << symbol << ".\n";
                }
            }
        }
    }
    return true; // Move the return statement here
}

void Board::initializeTextures(std::string name) {
    sf::Texture currentTexture;
    std::string filepath = name + ".png";
    if (currentTexture.loadFromFile(filepath)) {
        std::cout << "Loaded Texture: " << filepath << std::endl;
        m_textures.push_back(currentTexture);
    }
    else {
        std::cerr << "Failed to load image: " << filepath << std::endl;
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
    std::ofstream outFile(BOARD_FILE);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << BOARD_FILE << " for saving the board.\n";
        return false;
    }

    // Iterate through the board and save the corresponding character for each object
    for (int row = 0; row < m_height; ++row) {
        for (int col = 0; col < m_width; ++col) {
            outFile << m_boardState[row][col];
            std::cout << m_boardState[row][col] << " ";
        }
        std::cout << "\n";
        outFile << '\n';  // New line after each row
    }

    outFile.close();
    return true;
}
