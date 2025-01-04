#include "Board.h"

Board::Board() : width(10), height(10) {
    grid.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
}

Board::Board(int width, int height) : width(width), height(height) {
    grid.resize(static_cast<size_t>(width) * static_cast<size_t>(height));
}

size_t Board::getIndex(int x, int y) const {
    return static_cast<size_t>(y) * static_cast<size_t>(width) + static_cast<size_t>(x);
}

void Board::clearData() {
    grid.clear();
    width = 0;
    height = 0;
}


/// <summary>
/// made a change 
/// </summary>
/// <param name="cols"></param>
/// <param name="rows"></param>
/// <param name="W"></param>
/// <param name="H"></param>
void Board::initializeBoard(int cols, int rows, int W, int H) {
    const float toolbarHeight = 50.0f;  // Toolbar height (make this consistent across your code)
    width = cols;
    height = rows;
    grid.clear();
    grid.resize(static_cast<size_t>(width) * static_cast<size_t>(height));

    float cellW = static_cast<float>(W) / static_cast<float>(cols);
    float cellH = (static_cast<float>(H) - toolbarHeight) / static_cast<float>(rows);

    for (int row = 0; row < height; ++row) {  // Start at row 0
        for (int col = 0; col < width; ++col) {
            size_t index = getIndex(col, row);
            grid[index].setSize(sf::Vector2f(cellW, cellH));
            grid[index].setPosition(col * cellW, row * cellH + toolbarHeight);  // Add toolbar offset to y-position
            grid[index].setFillColor(sf::Color::White);
            grid[index].setOutlineColor(sf::Color::Black);
            grid[index].setOutlineThickness(1.f);
        }
    }
}


void Board::handleMouseClick(int mouseX, int mouseY, Object selectedObject) {
    int col = mouseX / 32;  // גובה כל תא
    int row = mouseY / 32;  // רוחב כל תא

    if (col >= 0 && col < width && row >= 0 && row < height) {
        size_t index = getIndex(col, row);

        auto textures = loadTextures();  // להטען הטקסטורות
        if (selectedObject == Object::DELETE) {
            grid[index].setFillColor(sf::Color::White);  // צבע רקע
        }
        else {
            grid[index].setTexture(&textures[selectedObject]);  // להחיל טקסטורה
        }
    }
}




///----------------------------------------------
bool Board::checkToolbarHover(int mouseY, int toolbarHeight) {
    return mouseY < toolbarHeight;
}

/// <summary>
/// made a change
/// </summary>
/// <param name="mouseX"></param>
/// <param name="mouseY"></param>
/// <param name="H"></param>
/// <param name="W"></param>
/// <param name="window"></param>
void Board::highlightCell(int mouseX, int mouseY, int H, int W, sf::RenderWindow* window) {

    static size_t lastHighlightedIndex = std::numeric_limits<size_t>::max(); // Track last highlighted cell
    const float toolbarHeight = 50.0f;  // Consistent toolbar height
    const float cellWidth = static_cast<float>(W) / width;  // Dynamically calculate cell width
    const float cellHeight = ((static_cast<float>(H)) - toolbarHeight) / height;  // Dynamic cell height 
    const float adjustHeight = mouseY - toolbarHeight;

    sf::Cursor customCursor;
    // Change cursor if mouse is in the toolbar area
    if (checkToolbarHover(mouseY, toolbarHeight)) {
        if (!isToolbarAreaHovered) {
            if (customCursor.loadFromSystem(sf::Cursor::Hand)) {
                window->setMouseCursor(customCursor); // Set custom cursor
            }
            isToolbarAreaHovered = true;
        }
        return; // No highlighting if in toolbar area
    }
    else {
        if (isToolbarAreaHovered) {
            sf::Cursor defaultCursor;
            if (defaultCursor.loadFromSystem(sf::Cursor::Arrow)) {
                window->setMouseCursor(defaultCursor); // Reset cursor to default
            }
            isToolbarAreaHovered = false;
        }
    }



    int col = static_cast<int>(mouseX / cellWidth);
    int row = static_cast<int>(adjustHeight / cellHeight);

    std::cout << "Mouse Position: (" << mouseX << ", " << mouseY << ")" << std::endl;
    std::cout << "Adjusted MouseY: " << adjustHeight << std::endl;
    std::cout << "Cell Dimensions: (" << cellWidth << ", " << cellHeight << ")" << std::endl;
    std::cout << "Target Cell: Column " << col << ", Row " << row << std::endl;

    // Check if the mouse is within grid bounds
    if (col >= 0 && col < width && row >= 0 && row < height) {
        size_t currentIndex = getIndex(col, row);

        if (lastHighlightedIndex != currentIndex) {
            if (lastHighlightedIndex != std::numeric_limits<size_t>::max()) {
                grid[lastHighlightedIndex].setFillColor(sf::Color::White); // Reset to default
                std::cout << "Reset cell at index " << lastHighlightedIndex << " to white." << std::endl;
            }

            // Highlight the new cell
            grid[currentIndex].setFillColor(sf::Color(200, 200, 200)); // Gray highlight
            std::cout << "Highlighted cell at index " << currentIndex << "." << std::endl;

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





sf::Color Board::getColorForObject(Object obj) {
    switch (obj) {
    case ROBOT: return sf::Color::Blue;    
    case GUARD: return sf::Color::Green;    
    case WALL: return sf::Color::Black;     
    case ROCK: return sf::Color::Yellow;   
    case DOOR: return sf::Color::Red;       
    default: return sf::Color::White;       
    }
}



bool Board::isRobotPresent() {
    for (const auto& cell : grid) {
        if (cell.getFillColor() == sf::Color::Blue) return true;
    }
    return false;
}

void Board::draw(sf::RenderWindow& window) {
    for (const auto& cell : grid) {
        window.draw(cell);
    }
}

bool Board::saveToFile() const {
    std::ofstream file(BOARD_FILE);
    if (!file.is_open()) {
        std::cerr << "Cannot open " << BOARD_FILE << " for writing!\n";
        return false;
    }

    file << height << " " << width << "\n";

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            size_t index = getIndex(col, row);
            file << grid[index].getFillColor().toInteger() << " ";
        }
        file << "\n";
    }

    return true;
}

bool Board::loadFromFile() {
    std::ifstream file(BOARD_FILE);
    if (!file.is_open()) {
        std::cerr << "Cannot open " << BOARD_FILE << " for reading!\n";
        return false;
    }

    int rows, cols;
    file >> rows >> cols;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    initializeBoard(cols, rows, cols * 32, rows * 32);

    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            size_t index = getIndex(col, row);
            int colorInt;
            file >> colorInt;
            sf::Color cellColor(colorInt);
            grid[index].setFillColor(cellColor);
        }
    }

    return true;
}

bool Board::CheckExistFile() {
    std::ifstream checkFile(BOARD_FILE);
    return checkFile.good();
}

bool Board::loadToolbarConfig(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Cannot open toolbar configuration file: " << filepath << "\n";
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        for (char c : line) {
            // Map the character to a corresponding filename
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


/// <summary>
/// made a change
/// </summary>
/// <returns></returns>
std::vector<sf::Texture> Board::loadTextures() {
    std::vector<sf::Texture> textures;

    std::cout << toolbarConfig.size() << std::endl;

    for (const auto& name : toolbarConfig) {
        sf::Texture texture;
        std::string filepath = name + ".png";

        if (texture.loadFromFile(filepath)) {
            texture.setSmooth(true);  // Enable smoothing for better visual appearance
            textures.push_back(std::move(texture));
            std::cout << "Loaded Texture: " << filepath << std::endl;
        }

        else {
            std::cerr << "Failed to load image: " << filepath << std::endl;
        }
    }

    return textures;
}
