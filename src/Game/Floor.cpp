#include "pch.h"
#include "Floor.h"
#include "mesh_addon.h"
#include "Resource.h"

Floor::Floor(Grid& grid)
{
	mesh_addon* tMesh;
	tMesh = new mesh_addon;
	tMesh->CreateRectangleBlock(static_cast<float>(grid.GetRowSize() * CELL_SIZE) / 2.0f, 0.5f, static_cast<float>(grid.GetColSize() * CELL_SIZE) / 2.0f);
	GetEntity()->pMesh = tMesh;
}

Floor::~Floor()
{
	delete GetEntity()->pMesh;
	delete GetEntity()->pMaterial;
}

void Floor::Init()
{
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = cpu::ToColor(201,201,0);
	GetEntity()->transform.pos.x = 0;
	GetEntity()->transform.pos.y = 0;
	GetEntity()->transform.pos.z = 0;
}
