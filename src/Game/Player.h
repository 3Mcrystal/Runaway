#pragma once
#include "BaseEntity.h"

class Player : public BaseEntity
{
public:
	Player();
	~Player();

	void Init() override;
	void Update(float dt) override;

private:

	std::pair<int, int> m_gridpos;
};