#include "pch.h"
#include "Bonus.h"
#include "Resource.h"
#include "Utils.h"
#include "Board.h"

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
	Board& board = Board::GetInstance();
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = { 255, 0, 255 };
	int row = Utils::GetRandomNumber(0, board.GetGrid().GetColSize() - 1);
	int col = Utils::GetRandomNumber(0, board.GetGrid().GetRowSize() - 1);
	GetTransform().pos = GridToWorld(row, col);
	GetTransform().pos.y = board.GetStandHeight(row, col);
}