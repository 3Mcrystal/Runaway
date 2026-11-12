#include "pch.h"
#include "BaseEntity.h"
#include "Resource.h"

BaseEntity::BaseEntity()
{
	m_pEntity = cpuEngine.CreateEntity();
}

BaseEntity::~BaseEntity()
{
	DestroyEntity();
}

cpu_entity* BaseEntity::GetEntity()
{
	return m_pEntity;
}

cpu_transform& BaseEntity::GetTransform()
{
	assert(m_pEntity != nullptr);

	return m_pEntity->transform;
}

void BaseEntity::SetEntity(cpu_entity* entity)
{
	m_pEntity = entity;
}

XMFLOAT3 BaseEntity::GridToWorld(int row, int col)
{
	Board& board = Board::GetInstance();

	return { col * CELL_SIZE + board.GetOrigin().x + 0.5f, 1.f, row * CELL_SIZE + board.GetOrigin().z + 0.5f };
}

std::pair<int, int> BaseEntity::WorldToGrid(XMFLOAT3 p)
{
	Board& board = Board::GetInstance();

	int row = roundf((p.z - board.GetOrigin().z - 0.5f) / CELL_SIZE);
	int col = roundf((p.x - board.GetOrigin().x - 0.5f) / CELL_SIZE);

	return std::pair<int, int>(row, col);
}

void BaseEntity::DestroyEntity()
{
	cpuEngine.Release(m_pEntity);
}