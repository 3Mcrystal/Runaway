#include "pch.h"
#include "Board.h"

#include <algorithm>

Board::Board()
{
    m_floor = new Floor(m_grid);
    m_wall = new Wall;
}

Board::~Board()
{
    delete m_floor;
    delete m_wall;
}

Grid& Board::GetGrid()
{
    return m_grid;
}

void Board::addEntranceAndExit() {

    // Set the entrance at the top (fixed)
    for (int x = 1; x < m_grid.GetRowSize(); x += 2) { // start at cell next to left/top border, move by unit of 2 to stay in structure
        if (m_grid[1][x]->GetIsWall() == false) { // Find a valid entry point
            m_grid[0][x]->SetIsWall(false);// Open entrance
            //mpPlayerNode = m_grid[0][x]; // Set player start position
            //mpCursorNode = m_grid[0][x]; // Set cursor start position
            break;
        }
    }

    // Find all possible exit locations on the bottom row and add that index to vector
    std::vector<int> validExits;
    for (int x = 1; x < m_grid.GetRowSize(); x += 2) {
        if (m_grid[m_grid.GetColSize() - 2][x]->GetIsWall() == false)
        {
            validExits.push_back(x); // Store all possible exits
        }
    }

    // Pick a random exit (index) from vector, if there are valid options
    if (!validExits.empty()) {
        int exitIndex = validExits[m_RNG() % validExits.size()]; // Select a random valid exit
        m_grid[m_grid.GetColSize() - 2][exitIndex]->SetIsWall(false); // Open exit
    }
}

void Board::CarveMaze(int x, int y) {

    // Moves two cells at a time to ensure clear paths are created and walls remain intact between paths.
    static const int dx[] = { 0,  0, -2,  2 }; // Movement offsets for Left & Right
    static const int dy[] = { -2, 2,  0,  0 }; // Movement offsets for Up & Down

    // List of direction indices (0: Up, 1: Down, 2: Left, 3: Right) - reused across function calls
    static std::vector<int> dirs = { 0, 1, 2, 3 };

    // A <algorithm> library function to randomly shuffle the directions to create unique maze structures on each generation
    std::shuffle(dirs.begin(), dirs.end(), m_RNG);

    // Mark the current cell/location (passed in as arg) as an open path (' ')
    m_grid[y][x]->SetIsWall(false);

    // lambda function to easily check the bounds of ny and nx(below) to ensure we are not at border or beyond
    auto isBounds = [](int ax, int bound)
        {
            return (ax > 0) && (ax < bound - 1);
        };

    // Iterate through shuffled directions to determine where to move next
    for (int dir : dirs)
    {
        // Calculate the new cell coordinates by moving two steps in the chosen direction
        // This moves in unit of 2
        int nx = x + dx[dir]; // new x position
        int ny = y + dy[dir]; // new y position

        // Check if the new cell is within the grid boundaries and is still a wall ('#')
        if (isBounds(ny, m_grid.GetColSize()) && isBounds(nx, m_grid.GetRowSize()) && m_grid[ny][nx]->GetIsWall())
        {

            // Carve a passage by opening the wall between the current cell and the new cell
            // We divide by 2 to get the actual neighbor cell (since we move by a unit of 2) and set element to space to connect passage with the current cell
            m_grid[y + dy[dir] / 2][x + dx[dir] / 2]->SetIsWall(false);

            // Recursively call CarveMaze on the new cell to continue carving paths - will carve down to last cell and return back up
            CarveMaze(nx, ny);
        }
    }
}

void Board::GenerateMaze()
{

    if (m_grid.GetColSize() <= 0 || m_grid.GetRowSize() <= 0)
    {
        std::cout << "Set the maze dimensions first!\n";
        return;
    }

    m_RNG.seed(std::random_device{}()); // generate new seed for randomness

    for (int i = 0; i < m_grid.GetColSize(); i++) // Fill entire grid with walls - initializes the vector to a new size and fills it
    {
        for (int j = 0; j < m_grid.GetRowSize(); j++)
        {
            m_grid[i][j]->SetIsWall(true);
        }
    }

    CarveMaze(1, 1); // Recursively carve path ways, randomly starting from (0,0) position (inside border)

    addEntranceAndExit(); // adds an entrance and exit to the existing new maze
}