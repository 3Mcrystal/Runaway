#include "pch.h"
#include "Floor.h"
#include "mesh_addon.h"
#include "Resource.h"

Floor::Floor(Grid& grid)
{
	mesh_addon* tMesh;
	tMesh = new mesh_addon;
	tMesh->CreateRectangleBlock(grid.GetColSize() * CELL_SIZE,grid.GetRowSize() * CELL_SIZE);
	GetEntity()->pMesh = tMesh;
}

Floor::~Floor()
{
}
