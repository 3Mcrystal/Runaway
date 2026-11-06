#pragma once
#include "Agent.h"
#include <memory>

class AgentManager
{
private:

	Grid& m_grid;
	Pathfinding m_pathfinding;
	std::vector<std::unique_ptr<Agent>> m_agents;

	float m_stepTime = 0.5f;
	float m_timer = 0.5f;

	void SolveFacing(Agent& a, Agent& b);

public:

	AgentManager(Grid& grid) :
		m_grid(grid)
	{ }

	Agent* CreateAgent(int x, int y); //out of grid/ wall/ occupied, nullprt
	Agent* GetAgentAt(int x, int y);

	void Update(float dt);
	void Step();
};
