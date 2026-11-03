#pragma once
#include "BaseEntity.h"

class Enemy : public BaseEntity
{
private:
public:
	Enemy();
	~Enemy();

	void Init() override;
};