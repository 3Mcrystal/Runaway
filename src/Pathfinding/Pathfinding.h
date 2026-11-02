#pragma once
#include "grid.h"
#include <iostream>
#include <vector>

struct Entity
{
	int xe = 0;
	int ye = 0;

	//Get
	int GetxP() { return xe; }
	int GetyP() { return ye; }

	//Set
	void SetxP(int value) { xe = value; }
	void SetyP(int value) { ye = value; }
};

struct Cursor
{
	int xc = 0;
	int yc = 0;

	//Get
	int GetxC() { return xc; }
	int GetyC() { return yc; }

	//Set
	void SetxC(int value) { xc = value; }
	void SetyC(int value) { yc = value; }
};

class Pathfinding
{
public:

	Pathfinding();
	~Pathfinding();

    void BFS(Grid& g, Node* start, Node* target);
    void Djikstra(Grid& g, Node* start, Node* target);

    bool GetAttainable() { return attainable; }
    const std::vector<Node*>& GetPath() const { return path; }
    int  GetCost() const { return cost; }

private:

	std::vector<Node*> path;

	Grid grid;
	Entity p1;
	Cursor c1;

	int deplacement = 0;

    int cost = 0;

	bool attainable = true;
    bool ActivateCost = true;
};
