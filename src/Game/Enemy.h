#pragma once
#include "BaseEntity.h"
#include <Agent.h>

class Player;

class Enemy : public BaseEntity
{
private:

	Agent* m_agent = nullptr;
	Player* m_target = nullptr; //player when in pursuit

	float m_speed = 4.f;
	float m_chaseTimer = 0.f;

public:

	Enemy();
	~Enemy();

	void SetAgent(Agent* agent);
	void SetTarget(Player* target) { m_target = target; }

	void Init() override;
	void Update(float dt) override;
};
