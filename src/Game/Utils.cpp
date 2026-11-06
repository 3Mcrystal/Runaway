#include "Utils.h"
#include "pch.h"

namespace Utils
{
	float LengthSqr(XMFLOAT3& vector3)
	{
		return vector3.x * vector3.x + vector3.y * vector3.y + vector3.z * vector3.z;
	};

	float Length(XMFLOAT3& vector3)
	{
		return sqrt(Utils::LengthSqr(vector3));
	};

	void Normalize(XMFLOAT3& vector3)
	{
		float len = Utils::Length(vector3);
		if (len <= 0)
			return;
		vector3.x /= len;
		vector3.y /= len;
		vector3.z /= len;
	}

	XMFLOAT3 Distance(const XMFLOAT3& vector3, const XMFLOAT3& other)
	{
		XMFLOAT3 result;
		result.x = other.x - vector3.x;
		result.y = other.y - vector3.y;
		result.z = other.z - vector3.z;
		return result;
	}

};