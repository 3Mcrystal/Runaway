#pragma once
#include "BaseEntity.h"

class Player : public BaseEntity
{
public:
	Player();
	~Player();

	bool IsAtApproximatePos(const XMFLOAT3& pos, const XMFLOAT3& dest, float approximate);
	
	//Get
	bool GetChangeDirection();
	bool GetIsMoving();

	XMFLOAT3 GetNewDir();
	XMFLOAT3 GetCurrentDir();

	//Set
	void SetChangeDirection(bool value);
	void SetIsMoving(bool value);

	void SetNewDir(XMFLOAT3 value);
	void SetCurrentDir(XMFLOAT3 value);

	void Init() override;
	void Update(float dt) override;

private:

	std::pair<int, int> m_gridpos;

	XMFLOAT3 m_NewDir;
	XMFLOAT3 m_CurrentDir;

	bool m_ChangeDirection = false;
	bool m_IsMoving = false;
};