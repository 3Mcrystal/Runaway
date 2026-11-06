#include "pch.h"
#include "Player.h"
#include "Board.h"
#include "Utils.h"

#include <Resource.h>

Player::Player()
{
	cpu_mesh* tMesh;
	tMesh = new cpu_mesh;
	tMesh->CreateSphere(0.5f,5,20);
	GetEntity()->pMesh = tMesh;
	GetEntity()->transform.dir = { 0,0,0 };
	m_TargetPos = { 0,0,0 };
}

Player::~Player()
{
	delete GetEntity()->pMesh;
	delete GetEntity()->pMaterial;
}

bool Player::GetChangeDirection()
{
	return m_ChangeDirection;
}

bool Player::GetIsMoving()
{
	return m_IsMoving;
}

XMFLOAT3 Player::GetNewDir()
{
	return m_NewDir;
}

XMFLOAT3 Player::GetCurrentDir()
{
	return m_CurrentDir;
}

void Player::SetChangeDirection(bool value)
{
	m_ChangeDirection = value;
}

void Player::SetIsMoving(bool value)
{
	m_IsMoving = value;
}

void Player::SetNewDir(XMFLOAT3 value)
{
	m_NewDir = value;
}

void Player::SetCurrentDir(XMFLOAT3 value)
{
	m_CurrentDir = value;
}

bool Player::MoveTowards(XMFLOAT3 target, float dt)
{
	Board& board = Board::GetInstance();

	float step = m_velocity * dt;

	XMFLOAT3 d = { m_TargetPos.x - GetTransform().pos.x, m_TargetPos.y - GetTransform().pos.y, m_TargetPos.z - GetTransform().pos.z };
	float distsquare = d.x * d.x + d.y * d.y + d.z * d.z;

	XMFLOAT3 distance = Utils::Distance(GetTransform().pos, target);
	Utils::Normalize(distance);
	
	if (abs(distance.x) > abs(distance.y) && abs(distance.x) > abs(distance.z))
	{
		distance.x < 0 ? -1 : 1;
		distance.y = 0;
		distance.z = 0;
	}
	if (abs(distance.y) > abs(distance.x) && abs(distance.y) > abs(distance.z))
	{
		distance.y < 0 ? -1 : 1;
		distance.x = 0;
		distance.z = 0;
	}
	if (abs(distance.z) > abs(distance.x) && abs(distance.z) > abs(distance.y))
	{
		distance.z < 0 ? -1 : 1;
		distance.x = 0;
		distance.y = 0;
	}

	SetNewDir(distance);
	SetCurrentDir(m_NewDir);
	if (distsquare <= step * step)
	{
		GetTransform().pos = target;
		return true;
	}
	else
	{
		GetTransform().Move(step);
		return false;
	}

}

void Player::Init()
{
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = cpu::ToColor(255, 0, 0);

	Board& board = Board::GetInstance();

	m_TargetPos = GetTransform().pos;

	auto [row, col] = WorldToGrid(GetTransform().pos);

	const XMFLOAT3 dirs[] = { { 1,0,0 },{ -1,0,0 },{ 0,0,1 },{ 0,0,-1 } };

	for (const XMFLOAT3& d : dirs)
	{
		if (board.IsWalkable(row + (int)d.z, col + (int)d.x))
		{
			m_CurrentDir = d;
			m_NewDir = d;
			GetTransform().dir = d;
			m_IsMoving = true;
			break;
		}
	}
}

void Player::Update(float dt)
{
	Board& board = Board::GetInstance();

	float step = m_velocity * dt;

	XMFLOAT3 d = { m_TargetPos.x - GetTransform().pos.x, m_TargetPos.y - GetTransform().pos.y, m_TargetPos.z - GetTransform().pos.z };
	float dist = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);

	//if (m_PositionQueue.empty() == false); à refacto

	if (dist <= step)
	{
		GetTransform().pos = m_TargetPos;

		auto [row, col] = WorldToGrid(GetTransform().pos);

		if (m_ChangeDirection && board.IsWalkable(row + (int)m_NewDir.z, col + (int)m_NewDir.x))
		{
			SetCurrentDir(m_NewDir);
			GetTransform().dir = m_CurrentDir;
			m_ChangeDirection = false;
		}

		int nextRow = row + (int)m_CurrentDir.z;
		int nextCol = col + (int)m_CurrentDir.x;

		bool canMove = (m_CurrentDir.x != 0 || m_CurrentDir.z != 0) && board.IsWalkable(nextRow, nextCol);

		m_IsMoving = canMove;

		if (canMove)
			m_TargetPos = GridToWorld(nextRow, nextCol);
		else
			m_TargetPos = GridToWorld(row, col);
	}

	if (m_IsMoving)
		GetTransform().Move(step);
}
