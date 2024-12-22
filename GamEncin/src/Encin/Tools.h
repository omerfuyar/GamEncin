#pragma once
#include <cmath>

namespace GamEncin
{
	class Vector2
	{
	public:
		float x,
			y,
			magnitude;

		Vector2(float x = 0, float y = 0) : x(x), y(y)
		{
			magnitude = GetMagnitude();
		}

		~Vector2() = default;

		static const Vector2 zero, one, up, right, down, left;

		float GetMagnitude()
		{
			magnitude = sqrt(x * x + y * y);
			return magnitude;
		}

		//Scales and returns the Vector2 magnitude of 1
		Vector2 Normalize()
		{
			x /= magnitude;
			y /= magnitude;
			magnitude = GetMagnitude();
			return Vector2(x, y);
		}

		//Returns the normalized version of the Vector2
		Vector2 Normalized()
		{
			Vector2 result = Vector2(x, y);
			result.Normalize();
			return result;
		}

		float Set(float newX, float newY)
		{
			x = newX;
			y = newY;
		}
	};
}