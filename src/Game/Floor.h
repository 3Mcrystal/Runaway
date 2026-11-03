#pragma once
#include "BaseEntity.h"
#include "mesh_addon.h"
#include <grid.h>

class Floor : public BaseEntity
{
public:
	Floor(Grid& grid);
	~Floor();

	void Init() override;

private:

};