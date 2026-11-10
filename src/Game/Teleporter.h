#include "BaseEntity.h"

class Teleporter : public BaseEntity
{
private:

	BaseEntity* Port1 = nullptr;
	BaseEntity* Port2 = nullptr;

public:

	void Init()override;
};