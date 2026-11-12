#pragma once
#include "BaseEntity.h"
#include <Agent.h>

class Player;

enum class EnemyState
{
	Patrol,
	Chase
};

class Enemy : public BaseEntity
{
private:

	Agent* m_agent = nullptr;
	Player* m_target = nullptr;

	EnemyState m_state = EnemyState::Patrol;

	//patrol route
	int m_startRow = 0;
	int m_startCol = 0;
	int m_destRow = 0;
	int m_destCol = 0;

	//Distances in cells
	float m_detectRadius = 5.f;   //strat chase
	float m_loseRadius = 8.f;     //chase stops
	float m_touchRadius = 0.7f;   //game over

	float m_cooldownTime = 5.f;
	float m_cooldown = 0.f;

	float m_thinkTimer = 0.f;     //the enemy updates its destination once per second

	float m_speed = 4.f;

	void StartChase();
	void StopChase();
	float DistanceToTarget();     //on the ground
	void SetColor(int r, int g, int b);

public:

	Enemy();
	~Enemy();

	void SetAgent(Agent* agent);
	void SetTarget(Player* target) { m_target = target; }

	void SetPatrolRoute(int destRow, int destCol);
	void ChangePatrolRoute(int startRow, int startCol, int destRow, int destCol);   //new start and destination while the game runs

	bool IsTouchingPlayer();

	void Init() override;
	void Update(float dt) override;
};
