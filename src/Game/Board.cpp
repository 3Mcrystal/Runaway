#include "pch.h"
#include "Board.h"
#include "Resource.h"
#include <algorithm>
#include "Player.h"
#include "Utils.h"

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
    //Clear pour recréer
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

bool Board::IsInBounds(int row, int col, int margin)
{
    return row >= margin && row < m_grid.GetColSize() - margin && col >= margin && col < m_grid.GetRowSize() - margin;
}

bool Board::IsWalkable(int row, int col)
{
    return IsInBounds(row, col, 1);
}

// renvoie la hauteur + le rayon du joueur pour éviter le clipping
float Board::GetStandHeight(int row, int col)
{
    if (IsInBounds(row, col, 0) == false)
        return FLOOR_TOP + PLAYER_RADIUS;

    Node* node = m_grid[row][col];

    const float top = node->GetIsWall() ? 2.f * node->GetLevel() : FLOOR_TOP;

    return top + PLAYER_RADIUS;
}

void Board::AddSpawn(Player& entity)
{
    if (m_grid[m_grid.GetColSize() / 2][m_grid.GetRowSize() / 2]->GetIsWall() == true)
    {
		m_grid[m_grid.GetColSize() / 2][m_grid.GetRowSize() / 2]->SetIsWall(false);// Creuse un trou au centre du labyrinthe pour spawn le joueur
        entity.GetTransform().SetPosition(m_grid.GetColSize() / 2 * CELL_SIZE + mOrigin.x + 0.5f, 1.f, m_grid.GetRowSize() / 2 * CELL_SIZE + mOrigin.z + 0.5f);
    }
    else
        entity.GetTransform().SetPosition(m_grid.GetColSize() / 2 * CELL_SIZE + mOrigin.x + 0.5f, 1.f, m_grid.GetRowSize() / 2 * CELL_SIZE + mOrigin.z + 0.5f);
}

void Board::CarveMaze(int x, int y)
{

    static const int dx[] = { 0,  0, -2,  2 };//direction de 2
    static const int dy[] = { -2, 2,  0,  0 };//direction de 2

    static std::vector<int> dirs = { 0, 1, 2, 3 };

    std::shuffle(dirs.begin(), dirs.end(), m_RNG);// mélange les directions

    m_grid[y][x]->SetIsWall(false);

    for (int dir : dirs)
    {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (IsInBounds(ny, nx, 1) && m_grid[ny][nx]->GetIsWall())
        {
            m_grid[y + dy[dir] / 2][x + dx[dir] / 2]->SetIsWall(false);//creuse le labyrinth
            CarveMaze(nx, ny); //appel la fonction récursivement pour continuer le creusage
        }
    }
}

//Enlève mur aléatoire
void Board::OpenMaze()
{
    //Uniquement cellule intérieur
    for (int i = 1; i < m_grid.GetColSize() - 1; i++)
    {
        for (int j = 1; j < m_grid.GetRowSize() - 1; j++)
        {
            // Skip cell si c'est du sol ou si m_RNG > % :
            if (m_grid[i][j]->GetIsWall() == false || (int)(m_RNG() % 100) >= m_wallPercent)
                continue;

            //si 1 voisins ou plus est une free cell?
            bool touchesFreeCell = m_grid[i - 1][j]->GetIsWall() == false || m_grid[i + 1][j]->GetIsWall() == false || m_grid[i][j - 1]->GetIsWall() == false || m_grid[i][j + 1]->GetIsWall() == false;

			//évite les espaces fermés
            if (touchesFreeCell)
                m_grid[i][j]->SetIsWall(false);
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

    OpenMaze();

    AddSpawn(entity);
}