#include "pch.h"
#include "Player.h"


Player::Player()
{
	cpu_mesh* tMesh;
	tMesh = new cpu_mesh;
	tMesh->CreateSphere(0.5f,5.f,20.f);
	GetEntity()->pMesh = tMesh;
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
