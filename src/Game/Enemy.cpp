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

void Enemy::SetPatrolRoute(int destRow, int destCol)
{
	m_startRow = m_agent->GetX();
	m_startCol = m_agent->GetY();
	m_destRow = destRow;
	m_destCol = destCol;

	m_agent->SetPatrol(true);
	m_agent->SetDestination(m_destRow, m_destCol);
}

void Enemy::SetColor(int r, int g, int b)
{
	if (GetEntity()->pMaterial != nullptr)
		GetEntity()->pMaterial->color = cpu::ToColor(r, g, b);
}

void Enemy::Init()
{
	GetEntity()->pMaterial = new cpu_material;
	SetColor(0, 200, 0);   //patrol color
}

float Enemy::DistanceToTarget()
{
	XMFLOAT3 a = GetTransform().pos;
	XMFLOAT3 b = m_target->GetTransform().pos;

	float dx = a.x - b.x;
	float dz = a.z - b.z;

	return sqrtf(dx * dx + dz * dz) / CELL_SIZE;
}

//don't touch from corridir
bool Enemy::IsTouchingPlayer()
{
	if (m_target == nullptr)
		return false;

	XMFLOAT3 a = GetTransform().pos;
	XMFLOAT3 b = m_target->GetTransform().pos;

	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;

	return sqrtf(dx * dx + dy * dy + dz * dz) < m_touchRadius * CELL_SIZE;
}

void Enemy::StartChase()
{
	m_state = EnemyState::Chase;
	m_thinkTimer = 0.f;   //instant

	m_agent->SetPatrol(false);
	SetColor(0, 0, 255);   //chase color
}

void Enemy::StopChase()
{
	m_state = EnemyState::Patrol;
	m_cooldown = m_cooldownTime;

	SetColor(0, 200, 0);
	m_agent->SetDestination(m_startRow, m_startCol);
}

void Enemy::Update(float dt)
{
	if (m_agent == nullptr)
		return;

	m_cooldown -= dt;
	m_thinkTimer -= dt;

	if (m_target != nullptr)
	{
		float distance = DistanceToTarget();

		if (m_state == EnemyState::Patrol)
		{
			//360 detection
			if (m_cooldown <= 0.f && distance < m_detectRadius)
				StartChase();
		}
		else if (distance > m_loseRadius)
		{
			StopChase();
		}
	}

	if (m_thinkTimer <= 0.f)
	{
		m_thinkTimer = 1.f;

		if (m_state == EnemyState::Chase)
		{
			//dest is player pos
			auto [row, col] = m_target->WorldToGrid(m_target->GetTransform().pos);
			m_agent->SetDestination(row, col);
		}
		else if (m_agent->HasPath() == false)
		{
			SetPatrolRoute(m_destRow, m_destCol);
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
