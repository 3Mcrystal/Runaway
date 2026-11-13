#include "Agent.h"

Agent::Agent(Grid* grid, Pathfinding* pathfinding, int x, int y) :
	m_grid(grid),
	m_pathfinding(pathfinding),
	m_x(x), m_y(y)
{
	//les autres agents l'evitent
	(*m_grid)[m_x][m_y]->SetIsBlocked(true);
}

bool Agent::SetDestination(int x, int y)
{
	//out of the grid
	if (x < 0 || x >= m_grid->GetColSize() || y < 0 || y >= m_grid->GetRowSize())
		return false;

	Node* node = (*m_grid)[x][y];

	//wall (can't climb), own cell, or cell taken by an agent
	if ((node->GetIsWall() && m_canClimb == false) || node->GetIsBlocked())
		return false;

	//au cas où
	int oldX = m_destX;
	int oldY = m_destY;

	m_destX = x;
	m_destY = y;

	//pas de chemin
	if (Replan() == false)
	{
		m_destX = oldX;
		m_destY = oldY;
		return false;
	}

	m_startX = m_x;
	m_startY = m_y;
	m_patrolEndX = x;
	m_patrolEndY = y;
	return true;
}

//Cells of other agents flagged as blocked,avoid
bool Agent::Replan()
{
	m_pathfinding->Djikstra(*m_grid, (*m_grid)[m_x][m_y], (*m_grid)[m_destX][m_destY], m_canClimb);

	if (m_pathfinding->GetAttainable() == false || m_pathfinding->GetPath().empty())
		return false;

	//repart de sa premiere case
	m_path = m_pathfinding->GetPath();
	m_indice = 0;
	return true;
}

void Agent::Stop()
{
	m_path.clear();
	m_indice = 0;
}

//libere la cases
void Agent::LeaveCell()
{
	(*m_grid)[m_x][m_y]->SetIsBlocked(false);
}

//m_indice compte les cases deja parcourues
void Agent::MoveToNext()
{
	m_x = m_path[m_indice]->GetX();
	m_y = m_path[m_indice]->GetY();
	m_indice++;

	(*m_grid)[m_x][m_y]->SetIsBlocked(true);
}

void Agent::OnArrival()
{
	if (m_patrol == false)
	{
		Stop();
		return;
	}

	//patrol go to the other end
	if (m_x == m_patrolEndX && m_y == m_patrolEndY)
	{
		m_destX = m_startX;
		m_destY = m_startY;
	}
	else
	{
		m_destX = m_patrolEndX;
		m_destY = m_patrolEndY;
	}

	if (Replan() == false)
		Stop();
}
