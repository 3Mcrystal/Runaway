#include "BaseEntity.h"

class Teleporter
{
private:

	BaseEntity* Port1 = nullptr;
	BaseEntity* Port2 = nullptr;

public:

	Teleporter();
	~Teleporter();

	void Init();
};