#include "pch.h"
#include "Board.h"
#include "Resource.h"
#include <algorithm>
#include "Player.h"

Board& Board::GetInstance()
{
    static Board* board = new Board;
    return *board;
}

void Board::DestroyInstance()
{
    delete &GetInstance();
}

Board::Board()
{
    gridW = m_grid.GetRowSize() * CELL_SIZE;
    gridH = m_grid.GetColSize() * CELL_SIZE;
    mOrigin = { (0 - gridW) * 0.5f, (0 - gridH) * 0.5f ,(0) };
}

Board::~Board()
{
    delete m_floor;
    delete m_wall;
}

void Board::Init()
{
    m_floor = new Floor(m_grid);
	m_floor->Init();

    for (int i = 0; i < m_grid.GetColSize(); i++)
    {
        for (int j = 0; j < m_grid.GetRowSize(); j++)
        {
            if (m_grid[i][j]->GetIsWall())
            {
                Wall* wall = new Wall;
                wall->Init(j * CELL_SIZE + mOrigin.x + 0.5f, 1.f, i * CELL_SIZE + mOrigin.y + 0.5f);
            }
        }
    }
}

Grid& Board::GetGrid()
{
    return m_grid;
}

XMFLOAT3 Board::GetOrigin()
{
    return mOrigin;
}

void Board::addEntranceAndExit(Player& entity) 
{
    if (m_grid[17][17]->GetIsWall() == true) 
    {
        m_grid[17][17]->SetIsWall(false);// Open entrance
        entity.GetTransform().SetPosition(17 * CELL_SIZE + mOrigin.x + 0.5f, 1.f, 17 * CELL_SIZE + mOrigin.y + 0.5f);
    }
    else
        entity.GetTransform().SetPosition(17 * CELL_SIZE + mOrigin.x + 0.5f, 1.f, 17 * CELL_SIZE + mOrigin.y + 0.5f);

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

void Board::GenerateMaze(Player& entity)
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

    addEntranceAndExit(entity); // adds an entrance and exit to the existing new maze
}