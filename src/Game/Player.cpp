#include "pch.h"
#include "Player.h"


Player::Player()
{
	cpu_mesh* tMesh;
	tMesh = new cpu_mesh;
	tMesh->CreateSphere(2.0f);
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
	GetEntity()->pMaterial->color = { 255, 0, 0};

	GetTransform().SetPosition( 0, 0, 0 );

}
