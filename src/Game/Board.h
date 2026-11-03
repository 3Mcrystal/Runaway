#pragma once
#include "Wall.h"
#include "Floor.h"
#include <grid.h>
#include <random>

class Board
{
private:

	Grid m_grid;
	std::mt19937 m_RNG;

	Floor* m_floor;
	Wall* m_wall;

public:

	Board();
	~Board();

	Grid& GetGrid();

	void CarveMaze(int x, int y);
	void addEntranceAndExit();
	void GenerateMaze();

};