#include "pch.h"
#include "Enemy.h"

Enemy::Enemy()
{
	mesh_addon* tMesh;
	tMesh = new mesh_addon;
	tMesh->CreateCube(0.4f);
	GetEntity()->pMesh = tMesh;
}

Enemy::~Enemy()
{
}
