#include "Board.h"

Board::Board() : m_width(10), m_height(10) {
    initializeTextures();
    grid.resize(static_cast<size_t>(m_width) * static_cast<size_t>(m_height));
    // Initialize m_boardState with default values (Object::DELETE)
    m_boardState.resize(m_height, std::vector<char>(m_width, ' '));
}

Board::Board(int width, int height) : m_width(width), m_height(height) {
    initializeTextures();
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
void Board::handleMouseClick(int mouseX, int mouseY, char selectedObject) {
    float cellWidth = grid[0].getSize().x; // Cell width
    float cellHeight = grid[0].getSize().y; // Cell height

    // Adjust mouseY to account for the toolbar
    int adjustedMouseY = mouseY - TOOLBAR_HEIGHT;

    int col = static_cast<int>(mouseX / cellWidth); // Calculate the column
    int row = static_cast<int>(adjustedMouseY / cellHeight); // Calculate the row

    if (col >= 0 && col < m_width && row >= 0 && row < m_height) {
        size_t index = getIndex(col, row);

        if (selectedObject == ' ') {
            grid[index].setFillColor(sf::Color::White); // Empty cell
            grid[index].setTexture(nullptr); // Remove texture
            m_boardState[row][col] = ' '; // Update board state to empty
        }
        else {
            // Load the texture associated with the selected object
            sf::Texture* texture = getTextureForObject(selectedObject);
            if (texture) {
                grid[index].setTexture(texture); // Set the new texture
                m_boardState[row][col] = selectedObject; // Update board state
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

//bool Board::isRobotPresent() {
//    for (const auto& row : m_boardState) {
//        for (const auto& cell : row) {
//            if (cell == Object::ROBOT) return true;
//        }
//    }
//    return false;
//}

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
                sf::Texture* texture = getTextureForObject(m_boardState[row][col]);
                if (texture) {
                    grid[index].setTexture(nullptr); // Clear the existing texture
                    grid[index].setTexture(texture); // Set the new texture
                }
                else {
                    std::cerr << "Error: Invalid texture for object type " << symbol << ".\n";
                }
            }
        }
    }
    return true; // Move the return statement here
}

void Board::initializeTextures() {
    if (!robotTexture.loadFromFile("Robot.png")) {
        std::cerr << "Failed to load robot texture\n";
    }
    if (!guardTexture.loadFromFile("Guard.png")) {
        std::cerr << "Failed to load guard texture\n";
    }
    if (!doorTexture.loadFromFile("Door.png")) {
        std::cerr << "Failed to load door texture\n";
    }
    if (!wallTexture.loadFromFile("Wall.png")) {
        std::cerr << "Failed to load wall texture\n";
    }
    if (!rockTexture.loadFromFile("Rock.png")) {
        std::cerr << "Failed to load rock texture\n";
    }
}

sf::Texture* Board::getTextureForObject(char selectedObject) {


        if (selectedObject == '/') {
			return &robotTexture;
		}
		else if (selectedObject == '!') {
			return &guardTexture;
		}
		else if (selectedObject == 'D') {
			return &doorTexture;
		}
		else if (selectedObject == '#') {
			return &wallTexture;
		}
		else if (selectedObject == '@') {
			return &rockTexture;
		}
        else 
            return nullptr;
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
