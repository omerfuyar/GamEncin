#pragma once
#include <cmath>
#include <vector>

using std::vector;

namespace GamEncin
{
	enum Layer
	{
		Default, IgnoreRay, UI, Player, Layer1, Layer2, Layer3
	};

	struct MathYaman
	{
	public:
		//Euler, PI, Earth's gravity, Gravitational constant
		static const float E, PI, GRAVITY, G;
	};

	class Vector2
	{
	public:
		static const Vector2 zero, one, up, right, down, left;

		float x, y, magnitude;

		Vector2(float x = 0, float y = 0) : x(x), y(y)
		{
			magnitude = GetMagnitude();
		}

		~Vector2() = default;

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

	class Vector3
	{
	public:
		static const Vector3 zero, one, up, right, down, left, forward, backward;
		
		float x, y, z, magnitude;

		Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z)
		{
			magnitude = GetMagnitude();
		}

		~Vector3() = default;

		float GetMagnitude()
		{
			magnitude = sqrt(x * x + y * y + z * z);
			return magnitude;
		}

		//Scales and returns the Vector3 magnitude of 1
		Vector3 Normalize()
		{
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
			magnitude = GetMagnitude();
			return Vector3(x, y, z);
		}

		//Returns the normalized version of the Vector2
		Vector3 Normalized()
		{
			Vector3 result = Vector3(x, y, z);
			result.Normalize();
			return result;
		}

		float Set(float newX, float newY, float newZ)
		{
			x = newX;
			y = newY;
			z = newZ;
		}

		static float* VerticesVectorToFloatArr(vector<Vector3> vertices);
	};
	
	class Vector4
	{
	public:
		static const Vector4 zero, one;
		
		float x, y, z,w;

		Vector4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}

		~Vector4() = default;

		float Set(float newX, float newY, float newZ,float newW)
		{
			x = newX;
			y = newY;
			z = newZ;
			w= newW;
		}
	};
}