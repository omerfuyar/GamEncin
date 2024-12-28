#pragma once
#include "Tools.h"

namespace GamEncin
{
	class Component 
	{
	public:
	};

	class Transform : Component
	{
		Vector3
			position,
			rotation,
			scale = Vector3::One();

		void Translate(Vector3 amount)
		{
			position += amount;
		}
	};

	class PsychicsBody : Component
	{
	public:
		float
			mass = 1,
			drag = 0,
			angularDrag = 0,
			gravityScale = 1;

	};

	class Renderer : Component
	{
	public:
		vector<Vector3> vertices;
		
	};
}