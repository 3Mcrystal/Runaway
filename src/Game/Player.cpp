#include "pch.h"
#include "Player.h"
#include "Board.h"

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

void Player::Init()
{
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = cpu::ToColor(255, 0, 0);
}

void Player::Update(float dt)
{
	Board& board = Board::GetInstance();

	int vitesse = 1;
	XMFLOAT3 Pos = { GetTransform().pos.x - board.GetOrigin().x - 0.5f , 1.f ,GetTransform().pos.z - board.GetOrigin().y - 0.5f};
	XMFLOAT3 DestPos = {Pos.x + board.GetOrigin().x + 0.5f , 1.f , Pos.z + board.GetOrigin().x + 0.5f};

	if (IsAtApproximatePos(Pos,DestPos,0.1f))
	{
		if ()
		{

		}

		if (m_ChangeDirection)
		{
			
		}

		GetTransform().Move(vitesse * dt);
	}
}
