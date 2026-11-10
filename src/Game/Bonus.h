#pragma once
#include "BaseEntity.h"

class Bonus :public BaseEntity
{
public:
	Bonus();
	~Bonus();

	void Init() override;
	void DestroyEntity()override;

	cpu_particle_emitter* GetParticleEmmiter();

private:
	cpu_particle_emitter* m_pEntityEmitter = nullptr;
};