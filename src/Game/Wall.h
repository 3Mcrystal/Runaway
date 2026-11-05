#pragma once
#include "BaseEntity.h"

class Wall : public BaseEntity
{
public:
	Wall();
	~Wall();

	void Init(float x, float y, float z);

private:

	int m_height = 1;

};