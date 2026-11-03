#pragma once
#include "BaseEntity.h"

class Wall : public BaseEntity
{
public:
	Wall();
	~Wall();

private:

	int m_height;

};