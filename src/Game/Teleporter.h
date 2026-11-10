#pragma once
#include "BaseEntity.h"
#include "mesh_addon.h"

class Teleporter
{
private:

	BaseEntity* mPort1 = nullptr;
	BaseEntity* mPort2 = nullptr;

	float m_teleportCooldown = 2.f;
	bool m_canTeleport = true;

public:

	Teleporter();
	~Teleporter();

	//Get
	BaseEntity* GetPort1();
	BaseEntity* GetPort2();

	bool CanTeleport();

	float GetTeleportCooldown();

	//Set

	void SetCanTeleport(bool canTeleport);
	void SetTeleportCooldown(float cooldown);
	void SetBaseTeleportCooldown();

	void Init();

};