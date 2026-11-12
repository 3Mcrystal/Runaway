#pragma once
#include "grid.h"
#include <iostream>
#include <vector>

class Pathfinding
{
public:

	Pathfinding();
	~Pathfinding();

    void BFS(Grid& g, Node* start, Node* target);
    void Djikstra(Grid& g, Node* start, Node* target, bool canClimb = false);

    bool GetAttainable() { return attainable; }
    const std::vector<Node*>& GetPath() const { return path; }
    int  GetCost() const { return cost; }

private:

	std::vector<Node*> path;

	Grid grid;

	int deplacement = 0;

    int cost = 0;

	bool attainable = true;
    bool ActivateCost = true;
};
