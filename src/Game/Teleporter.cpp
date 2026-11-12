#include "Teleporter.h"
#include "pch.h"
#include "Resource.h"
#include "Utils.h"

Teleporter::Teleporter()
{
	mPort1 = new BaseEntity;
	mPort2 = new BaseEntity;

	mesh_addon* tMesh;
	tMesh = new mesh_addon;
	tMesh->CreateRectangleBlock(CELL_SIZE / 2.f, 1.f, CELL_SIZE / 2.f);
	mPort1->GetEntity()->pMesh = tMesh;
	mPort2->GetEntity()->pMesh = tMesh;

	if (mPort1->GetEntity()->pMaterial == nullptr)
		mPort1->GetEntity()->pMaterial = new cpu_material;
	mPort1->GetEntity()->pMaterial->color = cpu::ToColor(223, 252, 3);

	if (mPort2->GetEntity()->pMaterial == nullptr)
		mPort2->GetEntity()->pMaterial = new cpu_material;
	mPort2->GetEntity()->pMaterial->color = cpu::ToColor(223, 252, 3);
}

Teleporter::~Teleporter()
{
	delete mPort1->GetEntity()->pMaterial;
	delete mPort1->GetEntity()->pMesh;
	delete mPort1;
	delete mPort2->GetEntity()->pMaterial;
	delete mPort2;
}

BaseEntity* Teleporter::GetPort1()
{
	return mPort1;
}

BaseEntity* Teleporter::GetPort2()
{
	return mPort2;
}

bool Teleporter::CanTeleport()
{
	return m_canTeleport;
}

float Teleporter::GetTeleportCooldown()
{
	return m_teleportCooldown;
}

void Teleporter::SetCanTeleport(bool canTeleport)
{
	m_canTeleport = canTeleport;
}

void Teleporter::SetBaseTeleportCooldown()
{
	m_teleportCooldown = 2.f;
}

void Teleporter::SetTeleportCooldown(float cooldown)
{
	m_teleportCooldown = cooldown;
}

void Teleporter::Init()
{
	Board& board = Board::GetInstance();

	int row = Utils::GetRandomNumber(1, board.GetGrid().GetColSize() - 2);
	int col = Utils::GetRandomNumber(1, board.GetGrid().GetRowSize() - 2);
	int row2 = Utils::GetRandomNumber(1, board.GetGrid().GetColSize() - 2);
	int col2 = Utils::GetRandomNumber(1, board.GetGrid().GetRowSize() - 2);

	if(row != row2 || col != col2)
	{
		board.GetGrid()[row][col]->SetIsWall(false);
		board.GetGrid()[row2][col2]->SetIsWall(false);

		mPort1->GetTransform().pos = mPort1->GridToWorld(row, col);
		mPort2->GetTransform().pos = mPort2->GridToWorld(row2, col2);
	}
	else
	{
		board.GetGrid()[board.GetGrid().GetRowSize() - 3][board.GetGrid().GetColSize() - 3]->SetIsWall(false);
		board.GetGrid()[3][3]->SetIsWall(false);

		mPort1->GetTransform().pos = mPort1->GridToWorld(board.GetGrid().GetRowSize() - 3, board.GetGrid().GetColSize() - 3);
		mPort2->GetTransform().pos = mPort2->GridToWorld(3, 3);
	}
}