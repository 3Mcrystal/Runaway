#pragma once
#include "Wall.h"
#include "Floor.h"
#include <grid.h>
#include <random>
#include <vector>
#include "Teleporter.h"

class Board
{
private:

	Grid m_grid;

	std::mt19937 m_RNG;

	Floor* m_floor = nullptr;

	inline static Board* s_instance = nullptr;

	std::vector<Wall*> m_walls;
	std::vector <BaseEntity*> m_BoardElementToUpdate;
	
	//walls% removed (0 to have normal)
	int m_wallPercent = 30;

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
	void PlaceTeleporter(int row, int col, Teleporter& teleporter);
	void OpenMaze();
	void AddSpawn(Player& entity);
	void GenerateMaze(Player& entity);

};