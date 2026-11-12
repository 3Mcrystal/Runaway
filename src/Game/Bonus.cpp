#include "pch.h"
#include "Bonus.h"
#include "Resource.h"
#include "Utils.h"
#include "Board.h"

Bonus::Bonus()
{
	mesh_addon* tMesh;
	tMesh = new mesh_addon;
	tMesh->CreateCube(0.4f);
	GetEntity()->pMesh = tMesh;
}

Bonus::~Bonus()
{
	m_pEntityEmitter = cpuEngine.Release(m_pEntityEmitter);
	delete GetEntity()->pMesh;
	delete GetEntity()->pMaterial;

}

void Bonus::Init()
{
	Board& board = Board::GetInstance();
	GetEntity()->pMaterial = new cpu_material;
	GetEntity()->pMaterial->color = { 255, 0, 255 };
	int row = Utils::GetRandomNumber(1, board.GetGrid().GetColSize() - 2);
	int col = Utils::GetRandomNumber(1, board.GetGrid().GetRowSize() - 2);
	GetTransform().pos = GridToWorld(row, col);
	GetTransform().pos.y = board.GetStandHeight(row, col);

	cpuEngine.GetParticleData()->Create(2000000);
	cpuEngine.GetParticlePhysics()->gy = -0.5f;
	m_pEntityEmitter = cpuEngine.CreateParticleEmitter();
	m_pEntityEmitter->pos = GetTransform().pos;
	m_pEntityEmitter->dir = { 0.f,1.f,0.f };
	m_pEntityEmitter->rate = 0.8f;
	m_pEntityEmitter->spread = 0.2f;
	m_pEntityEmitter->spawnRadius = 0.3f;
	m_pEntityEmitter->durationMax = 0.1f;
	m_pEntityEmitter->speedMin = 6.f;
	m_pEntityEmitter->colorMin = cpu::ToColor(255, 0, 255);
	m_pEntityEmitter->colorMax = cpu::ToColor(0, 255, 0);
}

cpu_particle_emitter* Bonus::GetParticleEmmiter()
{
	return m_pEntityEmitter;
}
