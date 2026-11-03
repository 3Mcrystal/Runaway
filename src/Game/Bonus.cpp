#include "pch.h"
#include "Bonus.h"

Bonus::Bonus()
{
	mesh_addon* tMesh;
	tMesh = new mesh_addon;
	tMesh->CreateCube(0.3f);
	GetEntity()->pMesh = tMesh;
}

Bonus::~Bonus()
{
	delete GetEntity()->pMesh;
	delete GetEntity()->pMaterial;
}

void Bonus::Init()
{
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = { 255, 255, 0 };
	GetTransform().SetPosition(0, 0, 0);
}
