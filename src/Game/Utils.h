#pragma once
#include "pch.h"

namespace Utils
{
	float Length(XMFLOAT3& vector3);

	float LengthSqr(XMFLOAT3& vector3);

	void Normalize(XMFLOAT3& vector3);

	XMFLOAT3 Distance(const XMFLOAT3& vector3,const XMFLOAT3& other );

	int GetRandomNumber(int min, int max);
}