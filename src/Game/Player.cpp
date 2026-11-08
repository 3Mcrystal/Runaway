#include "pch.h"
#include "Player.h"
#include "Board.h"
#include "Utils.h"

#include <Resource.h>

Player::Player()
{
	cpu_mesh* tMesh;
	tMesh = new cpu_mesh;
	tMesh->CreateSphere(PLAYER_RADIUS, 5, 20);
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
	float step = m_velocity * dt;

	XMFLOAT3 pos = GetTransform().pos;
	XMFLOAT3 d = { target.x - pos.x, target.y - pos.y, target.z - pos.z };
	float distsquare = d.x * d.x + d.y * d.y + d.z * d.z;

	if (distsquare <= step * step)
	{
		GetTransform().SetPosition(target.x, target.y, target.z);
		return true;
	}

	float dist = sqrtf(distsquare);
	GetTransform().SetPosition(pos.x + d.x / dist * step, pos.y + d.y / dist * step, pos.z + d.z / dist * step);
	return false;
}

void Player::QueueMove(int row, int col)
{
	Board& board = Board::GetInstance();

	XMFLOAT3 from = GetTransform().pos;
	XMFLOAT3 to = GridToWorld(row, col);
	to.y = board.GetStandHeight(row, col);

	const float eps = 0.01f;// permet de detecter une différence de hauteur entre deux cases en évitant les problèmes d'arrondi

	if (to.y > from.y + eps)		// montee : vertical puis horizontal
	{
		m_PositionQueue.push({ from.x, to.y, from.z });
		m_PositionQueue.push(to);
	}
	else if (to.y < from.y - eps)	// descente : horizontal puis vertical
	{
		m_PositionQueue.push({ to.x, from.y, to.z });
		m_PositionQueue.push(to);
	}
	else							// meme hauteur
		m_PositionQueue.push(to);
}

void Player::Init()
{
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = cpu::ToColor(255, 0, 0);

	Board& board = Board::GetInstance();

	m_TargetPos = GetTransform().pos;

	std::pair<int, int>PosGrid = WorldToGrid(GetTransform().pos);

	const XMFLOAT3 dirs[] = { { 1,0,0 },{ -1,0,0 },{ 0,0,1 },{ 0,0,-1 } };

	for (const XMFLOAT3& d : dirs)
	{
		if (board.IsWalkable(PosGrid.first + (int)d.z, PosGrid.second + (int)d.x))
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

	// En deplacement : on suit la file de waypoints, aucune decision tant qu'elle n'est pas vide
	if (!m_PositionQueue.empty())
	{
		if (MoveTowards(m_PositionQueue.front(), dt))
			m_PositionQueue.pop();

		if (!m_PositionQueue.empty())
			return;
	}

	std::pair<int,int> PosGrid = WorldToGrid(GetTransform().pos);

	if (m_ChangeDirection && board.IsWalkable(PosGrid.first + (int)m_NewDir.z, PosGrid.second + (int)m_NewDir.x))
	{
		SetCurrentDir(m_NewDir);
		GetTransform().dir = m_CurrentDir;
		m_ChangeDirection = false;
	}

	int nextRow = PosGrid.first + (int)m_CurrentDir.z;
	int nextCol = PosGrid.second + (int)m_CurrentDir.x;

	if ((m_CurrentDir.x != 0 || m_CurrentDir.z != 0) && board.IsWalkable(nextRow, nextCol))
	{
		QueueMove(nextRow, nextCol);
		m_IsMoving = true;
	}
	else
		m_IsMoving = false;
}