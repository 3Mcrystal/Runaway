#pragma once
#include <vector>
#include "grid.h"
#include "Pathfinding.h"

class Agent
{
private:

	Grid* m_grid = nullptr;
	Pathfinding* m_pathfinding = nullptr;

	int m_x, m_y;

	int m_destX = 0, m_destY = 0;
	int m_startX = 0, m_startY = 0;         //where the agent was
	int m_patrolEndX = 0, m_patrolEndY = 0; //the destination

	std::vector<Node*> m_path;
	int m_indice = 0;

	bool m_patrol = false;
	bool m_canClimb = false; //walls can be crossed

public:

	Node* nextCell = nullptr;
	bool wantsToMove = false;
	bool canMove = false;
	int partner = -1; //index of facing agent (-1, none)

	Agent(Grid* grid, Pathfinding* pathfinding, int x, int y);

	int GetX() const { return m_x; }
	int GetY() const { return m_y; }
	bool HasPath() const { return m_indice < (int)m_path.size(); }
	const std::vector<Node*>& GetPath() const { return m_path; }
	int GetIndice() const { return m_indice; }

	bool IsPatrol() const { return m_patrol; }
	void SetPatrol(bool value) { m_patrol = value; }

	bool CanClimb() const { return m_canClimb; }
	void SetCanClimb(bool value) { m_canClimb = value; }

	bool SetDestination(int x, int y); //false, request cancelled
	bool Replan();                     //false, no path, keep old
	void Stop();

	void LeaveCell();  //cell free again
	void MoveToNext(); //enter next cell
	void OnArrival();
};
