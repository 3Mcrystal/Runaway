#include "pch.h"
#include "Player.h"


Player::Player()
{
	cpu_mesh* tMesh;
	tMesh = new cpu_mesh;
	tMesh->CreateSphere(0.5f,15);
	GetEntity()->pMesh = tMesh;
}

Player::~Player()
{
}
