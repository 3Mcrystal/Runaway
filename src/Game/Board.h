#pragma once
#include "Wall.h"
#include "Floor.h"
#include <grid.h>
#include <random>
#include <vector>

class Board
{
private:

	Grid m_grid;

	std::mt19937 m_RNG;

	Floor* m_floor = nullptr;

	inline static Board* s_instance = nullptr;

	std::vector<Wall*> m_walls;
	std::vector <BaseEntity*> m_BoardElementToUpdate;

	XMFLOAT3 mOrigin;

	float gridW = 0.f;
	float gridH = 0.f;

public:

	static Board& GetInstance();
	void DestroyInstance();

	Board();
	~Board();

	void Init();
	void ClearWalls();

	//Get
	Floor& GetFloor() { return *m_floor; }
	Grid& GetGrid();
	XMFLOAT3 GetOrigin();

	// // //

	bool IsInBounds(int row, int col, int margin);

	bool IsWalkable(int row, int col);

	float GetStandHeight(int row, int col);

	void CarveMaze(int x, int y);
	void AddSpawn(Player& entity);
	void GenerateMaze(Player& entity);

};