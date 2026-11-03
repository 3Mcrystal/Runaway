#pragma once
#include <cpu-core/cpu_mesh.h>

class mesh_addon : public cpu_mesh
{
public:
	void CreateRectangleBlock(float halfwidth = 0.5f, float halfheight = 0.5f, float halfdepth = 0.5f, XMFLOAT3 color = CPU_WHITE);
};