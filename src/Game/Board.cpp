#include "pch.h"
#include "Board.h"
#include "Resource.h"
#include <algorithm>
#include "Player.h"

int GetRandomNumber(int min, int max)
{
    static std::random_device rd;

    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

Board& Board::GetInstance()
{
    if (s_instance == nullptr)
        s_instance = new Board;

    return *s_instance;
}

void Board::DestroyInstance()
{
    delete s_instance;
    s_instance = nullptr;

}

Board::Board()
{
    gridW = m_grid.GetRowSize() * CELL_SIZE;
    gridH = m_grid.GetColSize() * CELL_SIZE;
    mOrigin = { (0 - gridW) * 0.5f,(0), (0 - gridH) * 0.5f };
}

Board::~Board()
{
    for (Wall* w : m_BoardElementToUpdate)
    {
        delete w;
    }

    delete m_floor;
    ClearWalls();
}

void Board::ClearWalls()
{
    for (Wall* wall : m_walls)
        delete wall;

    m_walls.clear();
}

void Board::Init()
{
    //In case Init is called again?
    delete m_floor;
    ClearWalls();

    m_floor = new Floor(m_grid);
	m_floor->Init();

    for (int i = 0; i < m_grid.GetColSize(); i++)
    {
        for (int j = 0; j < m_grid.GetRowSize(); j++)
        {
            if (m_grid[i][j]->GetIsWall())
            {
                Wall* wall = new Wall;
                wall->Init(j * CELL_SIZE + mOrigin.x + 0.5f, 1.f, i * CELL_SIZE + mOrigin.z + 0.5f);
                m_walls.push_back(wall);
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

bool Board::IsInBounds(XMFLOAT3 pos)
{

    if (pos.x + GetOrigin().x + 0.5f < 0 ||
        pos.x + GetOrigin().x + 0.5f > GetGrid().GetRowSize() - 1 ||
        pos.z + GetOrigin().z + 0.5f < 0 ||
        pos.z + GetOrigin().z + 0.5f > GetGrid().GetColSize() - 1)
        return false;

    return true;
}

bool Board::IsWalkable(int row, int col)
{
	// les murs sont walkable
	if (row < 0 || row >= m_grid.GetColSize())
		return false;
    else if(col < 0 || col >= m_grid.GetRowSize())
        return false;
    return true;
}

void Board::AddSpawn(Player& entity) 
{
    if (m_grid[m_grid.GetColSize() / 2][m_grid.GetRowSize() / 2]->GetIsWall() == true) 
    {
        m_grid[m_grid.GetColSize() / 2][m_grid.GetRowSize() / 2]->SetIsWall(false);// Open entrance
        entity.GetTransform().SetPosition(m_grid.GetColSize() / 2 * CELL_SIZE + mOrigin.x + 0.5f, 1.f, m_grid.GetRowSize() / 2 * CELL_SIZE + mOrigin.z + 0.5f);
    }
    else
        entity.GetTransform().SetPosition(m_grid.GetColSize() / 2 * CELL_SIZE + mOrigin.x + 0.5f, 1.f, m_grid.GetRowSize() / 2 * CELL_SIZE + mOrigin.z + 0.5f);
}

void Board::CarveMaze(int x, int y) 
{

    static const int dx[] = { 0,  0, -2,  2 };
    static const int dy[] = { -2, 2,  0,  0 };

    static std::vector<int> dirs = { 0, 1, 2, 3 };

    std::shuffle(dirs.begin(), dirs.end(), m_RNG);

    m_grid[y][x]->SetIsWall(false);

    auto isBounds = [](int ax, int bound)
        {
            return (ax > 0) && (ax < bound - 1);
        };

    for (int dir : dirs)
    {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (isBounds(ny, m_grid.GetColSize()) && isBounds(nx, m_grid.GetRowSize()) && m_grid[ny][nx]->GetIsWall())
        {
            m_grid[y + dy[dir] / 2][x + dx[dir] / 2]->SetIsWall(false);

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

    m_RNG.seed(std::random_device{}());

    for (int i = 0; i < m_grid.GetColSize(); i++)
    {
        for (int j = 0; j < m_grid.GetRowSize(); j++)
        {
            m_grid[i][j]->SetIsWall(true);
        }
    }

    CarveMaze(1, 1);

    AddSpawn(entity);

    for (int i = 0; i < m_grid.GetColSize(); i++)
    {
        for (int j = 0; j < m_grid.GetRowSize(); j++)
        {
            if (m_grid[i][j]->GetIsWall())
            {
                m_grid[i][j]->SetLevel(GetRandomNumber(1, 2));
            }
        }
    }
}