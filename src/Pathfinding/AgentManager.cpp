#include "AgentManager.h"

//Cree un agent sur la case  / le garde dans la liste
//nullptr si case hors grille / un mur, ou deja prise
Agent* AgentManager::CreateAgent(int x, int y)
{
	if (x < 0 || x >= m_grid.GetColSize() || y < 0 || y >= m_grid.GetRowSize())
		return nullptr;

	if (m_grid[x][y]->GetIsWall() || m_grid[x][y]->GetIsBlocked())
		return nullptr;

	m_agents.push_back(std::make_unique<Agent>(&m_grid, &m_pathfinding, x, y)); //unique_ptr
	return m_agents.back().get();
}

Agent* AgentManager::GetAgentAt(int x, int y)
{
	for (auto& a : m_agents)
	{
		if (a->GetX() == x && a->GetY() == y)
			return a.get();
	}
	return nullptr;
}

void AgentManager::Update(float dt)
{
	m_timer -= dt;

	if (m_timer <= 0.f)
	{
		m_timer = m_stepTime;
		Step();
	}
}

void AgentManager::Step()
{
	int n = (int)m_agents.size();

	//each agent with a path wants its next cell
	for (int i = 0; i < n; i++)
	{
		Agent& a = *m_agents[i];

		a.wantsToMove = a.HasPath();
		a.canMove = a.wantsToMove;
		a.partner = -1;

		if (a.wantsToMove)
			a.nextCell = a.GetPath()[a.GetIndice()];
	}

	//Two agents want each other's cell
	//nobody moves
	for (int i = 0; i < n; i++)
	{
		Agent& a = *m_agents[i];

		if (a.wantsToMove == false)
			continue;

		Agent* other = GetAgentAt(a.nextCell->GetX(), a.nextCell->GetY());

		if (other != nullptr && other->wantsToMove &&
			other->nextCell->GetX() == a.GetX() && other->nextCell->GetY() == a.GetY())
		{
			for (int j = 0; j < n; j++)
			{
				if (m_agents[j].get() == other)
					a.partner = j;
			}
			a.canMove = false;
		}
	}

	//Cancel the moves into cell still taken / wanted
	//by agent before in the list
	//loop until no changes
	bool changed = true;

	while (changed)
	{
		changed = false;

		for (int i = 0; i < n; i++)
		{
			Agent& a = *m_agents[i];

			if (a.canMove == false)
				continue;

			bool blocked = false;

			//un agent est sur la case voulue
			Agent* other = GetAgentAt(a.nextCell->GetX(), a.nextCell->GetY());

			if (other != nullptr && other->canMove == false)
				blocked = true;

			//agent avant lui dans la liste va prendre la même case
			for (int k = 0; k < i; k++)
			{
				if (m_agents[k]->canMove && m_agents[k]->nextCell == a.nextCell)
					blocked = true;
			}

			if (blocked)
			{
				a.canMove = false;
				changed = true;
			}
		}
	}

	//Find a new route or both stop
	for (int i = 0; i < n; i++)
	{
		int j = m_agents[i]->partner;

		if (j > i)
			SolveFacing(*m_agents[i], *m_agents[j]);
	}

	//Moves, leaves first, then enters
	std::vector<Agent*> moved;

	for (int i = 0; i < n; i++)
	{
		if (m_agents[i]->wantsToMove && m_agents[i]->canMove)
		{
			m_agents[i]->LeaveCell();
			moved.push_back(m_agents[i].get());
		}
	}

	for (Agent* a : moved)
		a->MoveToNext();

	//arrivals are checked after all moves
	//the paths avoid the final positions
	for (Agent* a : moved)
	{
		if (a->HasPath() == false)
			a->OnArrival();
	}
}

//a goes around b,
//b goes around a it's new path
void AgentManager::SolveFacing(Agent& a, Agent& b)
{
	//b cell blocked, a avoids it
	bool okA = a.Replan();
	bool okB = false;

	if (okA)
	{
		//b also avoid the new path of a
		std::vector<Node*> marked;

		for (Node* node : a.GetPath())
		{
			if (node->GetIsBlocked() == false)
			{
				node->SetIsBlocked(true);
				marked.push_back(node);
			}
		}

		okB = b.Replan();

		for (Node* node : marked)
			node->SetIsBlocked(false);
	}

	if (okA == false || okB == false)
	{
		a.Stop();
		b.Stop();
	}

	//solved
	a.wantsToMove = false;
	b.wantsToMove = false;
}
