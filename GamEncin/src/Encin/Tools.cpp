#include "Tools.h"

namespace GamEncin
{
	const Vector2
		Vector2::zero = Vector2(0, 0),
		Vector2::one = Vector2(1, 1),
		Vector2::up = Vector2(0, 1),
		Vector2::right = Vector2(1, 0),
		Vector2::down = Vector2(0, -1),
		Vector2::left = Vector2(-1, 0);
	
	const Vector3
		Vector3::zero = Vector3(0, 0,0),
		Vector3::one = Vector3(1, 1,1),
		Vector3::up = Vector3(0, 1,0),
		Vector3::right = Vector3(1, 0,0),
		Vector3::down = Vector3(0, -1,0),
		Vector3::left = Vector3(-1, 0,0),
		Vector3::forward = Vector3(0, 0,1),
		Vector3::backward = Vector3(0, 0,-1);
}