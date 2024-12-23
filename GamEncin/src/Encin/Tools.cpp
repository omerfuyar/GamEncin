#pragma once
#include "SceneManager.h"
#include "Tools.h"

namespace GamEncin
{
	vector<shared_ptr<Object>> objects;
	unique_ptr<SceneManager> INSTANCE = nullptr;
	
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

	const float MathYaman::E = 2.7182817F,
		MathYaman::PI = 3.1415927F,
		MathYaman::G = (float) (6.67 / 1e11),
		MathYaman::GRAVITY = 9.81F;
}