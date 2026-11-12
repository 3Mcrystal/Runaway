#pragma once

class BaseEntity
{
private:

	cpu_entity* m_pEntity = nullptr;

public:

	//Constructor
	BaseEntity();

	virtual ~BaseEntity();

	//Get
	cpu_entity* GetEntity();
	cpu_transform& GetTransform();

	//Set
	void SetEntity(cpu_entity* entity);

	// // // //

	XMFLOAT3 GridToWorld(int row, int col);
	std::pair<int, int> WorldToGrid(XMFLOAT3 p);

	virtual void DestroyEntity();

	virtual void Init() {};
	virtual void Update(float dt) {};
	virtual void Reset() {};
};