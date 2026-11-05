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

	Floor* m_floor = nullptr;
	Wall* m_wall = nullptr;

	std::vector<BaseEntity*> m_BoardElementToUpdate;

	XMFLOAT3 mOrigin;

	float gridW = 0.f;
	float gridH = 0.f;

public:

	static Board &GetInstance();
	void DestroyInstance();

	Board();
	~Board();

	//Get
	Floor& GetFloor() { return *m_floor; }
	Wall& GetWall() { return *m_wall; }
	Grid& GetGrid();
	XMFLOAT3 GetOrigin();

	//Set


	void Init();


	void CarveMaze(int x, int y);
	void addEntranceAndExit(Player& entity);
	void GenerateMaze(Player& entity);

};