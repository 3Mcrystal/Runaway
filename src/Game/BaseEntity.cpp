#include "pch.h"
#include "BaseEntity.h"

BaseEntity::BaseEntity(cpu_mesh mesh, cpu_material material, cpu_entity* entity, cpu_particle_emitter* particle_emitter)
	: m_pEntity(entity)
{}

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

void BaseEntity::DestroyEntity()
{
	cpuEngine.Release(m_pEntity);
}