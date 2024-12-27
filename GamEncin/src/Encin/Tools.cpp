#pragma once
#include "Components.h"
#include "SceneManager.h"
#include "Tools.h"

namespace GamEncin
{
	const float MathYaman::E = 2.7182817F,
		MathYaman::PI = 3.1415927F,
		MathYaman::G = (float) (6.67 / 1e11),
		MathYaman::GRAVITY = 9.81F;

	const Vector2
		Vector2::zero = Vector2(0, 0),
		Vector2::one = Vector2(1, 1),
		Vector2::up = Vector2(0, 1),
		Vector2::right = Vector2(1, 0),
		Vector2::down = Vector2(0, -1),
		Vector2::left = Vector2(-1, 0);

	const Vector3
		Vector3::zero = Vector3(0, 0, 0),
		Vector3::one = Vector3(1, 1, 1),
		Vector3::up = Vector3(0, 1, 0),
		Vector3::right = Vector3(1, 0, 0),
		Vector3::down = Vector3(0, -1, 0),
		Vector3::left = Vector3(-1, 0, 0),
		Vector3::forward = Vector3(0, 0, 1),
		Vector3::backward = Vector3(0, 0, -1);

	float* Vector3::VerticesVectorToFloatArr(vector<Vector3> vertices)
	{
		size_t size = vertices.size() * 3; // coordinate count
		float* result = new float[size];
		
		int j = 0;
		for(int i = 0; i < size; i += 3)
		{
			Vector3 currV3 = vertices.at(j);
			result[i] = currV3.x;
			result[i + 1] = currV3.y;
			result[i + 2] = currV3.z;
			j++;
		}

		return result;
	}

	const Vector4
		Vector4::zero = Vector4(0, 0, 0, 0),
		Vector4::one = Vector4(1, 1, 1, 1);
}