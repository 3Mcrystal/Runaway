#include "pch.h"
#include "Player.h"
#include "Board.h"
#include <Resource.h>

Player::Player()
{
	cpu_mesh* tMesh;
	tMesh = new cpu_mesh;
	tMesh->CreateSphere(0.5f,5,20);
	GetEntity()->pMesh = tMesh;
	GetEntity()->transform.dir = { 0,0,0 };
}

Player::~Player()
{
	delete GetEntity()->pMesh;
	delete GetEntity()->pMaterial;
}

bool Player::IsAtApproximatePos(const XMFLOAT3& pos, const XMFLOAT3& dest, float approximate)
{
	float dx = dest.x - pos.x;
	float dy = dest.y - pos.y;
	float dz = dest.z - pos.z;

	return  dx <= approximate && dx >= -approximate && dy <= approximate && dy >= -approximate && dz <= approximate && dz >= - approximate;
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

XMFLOAT3 Player::GridToWorld(int row, int col)
{
	Board& board = Board::GetInstance();

	return { col * CELL_SIZE + board.GetOrigin().x + 0.5f, 1.f, row * CELL_SIZE + board.GetOrigin().z + 0.5f };
}

std::pair<int, int> Player::WorldToGrid(XMFLOAT3 p)
{
	Board& board = Board::GetInstance();

	int row = (int)((p.z - board.GetOrigin().z - 0.5f) / CELL_SIZE);
	int col = (int)((p.x - board.GetOrigin().x - 0.5f) / CELL_SIZE);

	return std::pair<int, int>(row, col);
}

void Player::Init()
{
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = cpu::ToColor(255, 0, 0);
}

void Player::Update(float dt)
{
	Board& board = Board::GetInstance();

	XMFLOAT3 Pos = { GetTransform().pos.x - board.GetOrigin().x - 0.5f , 1.f ,GetTransform().pos.z - board.GetOrigin().z - 0.5f};
	XMFLOAT3 DestPos = GetTransform().pos;

	if (WorldToGrid(GetTransform().pos) == GridToWorld(WorldToGrid(GetTransform().pos).first, WorldToGrid(GetTransform().pos).second))
	{
		if (m_ChangeDirection)
		{
			if (board.IsWalkable((int)(Pos.z + m_NewDir.z), (int)(Pos.x + m_NewDir.x)))
			{
				SetCurrentDir(m_NewDir);
				m_ChangeDirection = false;
				m_IsMoving = true;
			}
		}
		else
		{
			if (board.IsWalkable((int)(Pos.z + m_CurrentDir.z), (int)(Pos.x + m_CurrentDir.x)) == false)
			{
				m_IsMoving = false;
			}
		}
	}

	if (m_IsMoving)
		GetTransform().Move(m_velocity * dt);

	//if (IsAtApproximatePos(Pos, DestPos, 0.1f))
	//{
	//	if (IsInBounds(Pos))
	//		m_IsMoving = false;

	//	if (m_ChangeDirection)
	//	{
	//		if (IsInBounds(Pos + m_NewDir))
	//			return;

	//		SetNewDir(m_NewDir);
	//		m_ChangeDirection = false;
	//		m_IsMoving = true;
	//	}
	//}

}
