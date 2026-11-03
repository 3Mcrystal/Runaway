#include "pch.h"
#include "Wall.h"
#include "Resource.h"

Wall::Wall()
{
	m_height = 1;
	cpu_mesh* tMesh;
	tMesh = new cpu_mesh;
	tMesh->CreateCube(CELL_SIZE / 2);
	GetEntity()->pMesh = tMesh;
}

Wall::~Wall()
{
}
