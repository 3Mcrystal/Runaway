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
}
