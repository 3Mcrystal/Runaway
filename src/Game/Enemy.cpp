#include "pch.h"
#include "Enemy.h"
#include "Player.h"
#include "Board.h"
#include <Resource.h>

Enemy::Enemy()
{
	mesh_addon* tMesh;
	tMesh = new mesh_addon;
	tMesh->CreateCube(0.4f);
	GetEntity()->pMesh = tMesh;
}

Enemy::~Enemy()
{
	delete GetEntity()->pMesh;
	delete GetEntity()->pMaterial;
}

void Enemy::SetAgent(Agent* agent)
{
	m_agent = agent;

	GetTransform().pos = GridToWorld(m_agent->GetX(), m_agent->GetY());
}

void Enemy::Init()
{
	GetEntity()->pMaterial = new cpu_material;

	if (m_target != nullptr)
		GetEntity()->pMaterial->color = cpu::ToColor(0, 0, 255);   //pursuit color
	else
		GetEntity()->pMaterial->color = cpu::ToColor(0, 200, 0);   //patrol color
}

void Enemy::Update(float dt)
{
	if (m_agent == nullptr)
		return;

	//Pursuit (dest is player pos)
	if (m_target != nullptr)
	{
		m_chaseTimer -= dt;

		if (m_chaseTimer <= 0.f)
		{
			m_chaseTimer = 1.f;

			auto [row, col] = m_target->WorldToGrid(m_target->GetTransform().pos);
			m_agent->SetDestination(row, col);
		}
	}

	XMFLOAT3 goal = GridToWorld(m_agent->GetX(), m_agent->GetY());
	XMFLOAT3& pos = GetTransform().pos;

	float dx = goal.x - pos.x;
	float dz = goal.z - pos.z;
	float dist = sqrtf(dx * dx + dz * dz);
	float step = m_speed * dt;

	if (dist <= step)
	{
		pos = goal;
	}
	else
	{
		pos.x += dx / dist * step;
		pos.z += dz / dist * step;
	}
}
