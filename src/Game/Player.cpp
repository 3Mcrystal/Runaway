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

void Player::Init()
{
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = cpu::ToColor(255, 0, 0);
}

void Player::Update(float dt)
{
	int a = 1;
	GetTransform().Move(a * dt);
	if()
}
