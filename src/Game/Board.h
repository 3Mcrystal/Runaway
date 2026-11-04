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

	std::vector<BaseEntity*> m_BoardElementToUpdate;

	XMFLOAT3 mOrigin = { 0.f, 0.f ,0.f };

	float gridW = 0.f;
	float gridH = 0.f;

public:

	Board();
	~Board();

	Floor& GetFloor() { return *m_floor; }
	Wall& GetWall() { return *m_wall; }

	void Init();

	Grid& GetGrid();

	void CarveMaze(int x, int y);
	void addEntranceAndExit(Player& entity);
	void GenerateMaze(Player& entity);

};