#include "pch.h"
#include "Wall.h"
#include "Resource.h"
#include "mesh_addon.h"

Wall::Wall()
{
	m_height = 1;
	mesh_addon* tMesh;
	tMesh = new mesh_addon;
	tMesh->CreateRectangleBlock(CELL_SIZE/2, m_height, CELL_SIZE/2);
	GetEntity()->pMesh = tMesh;
}

Wall::~Wall()
{
	delete GetEntity()->pMesh;
	delete GetEntity()->pMaterial;
}

void Wall::Init(float x, float y, float z)
{
	if (GetEntity()->pMaterial == nullptr)
		GetEntity()->pMaterial = new cpu_material;

	GetEntity()->pMaterial->color = cpu::ToColor(0, 201, 201);
	GetEntity()->transform.pos.x = x;
	GetEntity()->transform.pos.y = y;
	GetEntity()->transform.pos.z = z;
}