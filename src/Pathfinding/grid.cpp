#include "grid.h"
#include <corecrt_malloc.h>
#include <cstdlib>
#include <iostream>

Grid::Grid()
{
	grid.resize(lines);
    for (int i = 0; i < GetColSize(); i++)
    {
        grid[i].resize(row);
        for (int j = 0; j < GetRowSize(); j++)
        {
            grid[i][j] = new Node();
            grid[i][j]->SetX(i);
            grid[i][j]->SetY(j);
            grid[i][j]->SetIsWall(false);
            grid[i][j]->SetIsVisited(false);
        }
    }
}

Grid::~Grid()
{
    for (auto& line : grid)
        for (Node* n : line)
            delete n;
}

Node* Grid::GetNode(int col, int row)
{
    return grid[col][row];
}

std::vector<Node*>& Grid::operator[](size_t row)
{
	return grid[row];
}