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

	m_agent->SetCanClimb(true);

	GetTransform().pos = GridToWorld(m_agent->GetX(), m_agent->GetY());
	GetTransform().pos.y = Board::GetInstance().GetStandHeight(m_agent->GetX(), m_agent->GetY());
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

void Enemy::ChangePatrolRoute(int startRow, int startCol, int destRow, int destCol)
{
	m_startRow = startRow;
	m_startCol = startCol;
	m_destRow = destRow;
	m_destCol = destCol;

	//go to new start when chase is done
	if (m_state == EnemyState::Patrol)
	{
		m_agent->SetPatrol(false);
		m_agent->SetDestination(m_startRow, m_startCol);
	}
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
			//No path, go back to start cell
			//SetPatrolRoute takes current cell as start
			if (m_agent->GetX() == m_startRow && m_agent->GetY() == m_startCol)
			{
				SetPatrolRoute(m_destRow, m_destCol);
			}
			else
			{
				m_agent->SetPatrol(false);
				m_agent->SetDestination(m_startRow, m_startCol);
			}
		}
	}

	XMFLOAT3 goal = GridToWorld(m_agent->GetX(), m_agent->GetY());
	goal.y = Board::GetInstance().GetStandHeight(m_agent->GetX(), m_agent->GetY());

	XMFLOAT3& pos = GetTransform().pos;

	//Like the player
	XMFLOAT3 target = goal;
	float flat = sqrtf((goal.x - pos.x) * (goal.x - pos.x) + (goal.z - pos.z) * (goal.z - pos.z));

	if (goal.y > pos.y + 0.01f)
		target = { pos.x, goal.y, pos.z };
	else if (goal.y < pos.y - 0.01f && flat > 0.01f)
		target = { goal.x, pos.y, goal.z };

	float dx = target.x - pos.x;
	float dy = target.y - pos.y;
	float dz = target.z - pos.z;
	float dist = sqrtf(dx * dx + dy * dy + dz * dz);
	float step = m_speed * dt;

	if (dist <= step)
	{
		pos = target;
	}
	else
	{
		pos.x += dx / dist * step;
		pos.y += dy / dist * step;
		pos.z += dz / dist * step;
	}

	auto [row, col] = WorldToGrid(pos);
	float ground = Board::GetInstance().GetStandHeight(row, col);

	if (pos.y < ground)
		pos.y = ground;
}
