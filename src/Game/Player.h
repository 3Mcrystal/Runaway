#pragma once
#include "BaseEntity.h"
#include <queue>

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

	// // // // //

	XMFLOAT3 GridToWorld(int row, int col);
	std::pair<int, int> WorldToGrid(XMFLOAT3 p);

	bool MoveTowards(XMFLOAT3 target, float dt);

	void Init() override;
	void Update(float dt) override;

private:

	std::pair<int, int> m_PosInGrid;
	std::pair<int, int> m_TargetPosInGrid;

	std::queue<XMFLOAT3> m_climbingPositions;

	XMFLOAT3 m_TargetPos;

	XMFLOAT3 m_NewDir;
	XMFLOAT3 m_CurrentDir;

	float m_velocity = 5.0f;

	bool m_ChangeDirection = false;
	bool m_IsMoving = true;
};