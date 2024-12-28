#pragma once
#include "Tools.h"

namespace GamEncin
{
	class Component
	{
	public:
	};

	class Transform : public Component
	{
	public:
		Vector3
			position,
			rotation,
			scale = Vector3::One();

		// Adds to position
		void Translate(Vector3 amount);

		// Adds to rotation
		void Rotate(Vector3 amount);

		// adds to scale
		void Scale(Vector3 amount);
	};

	class PsychicsBody : public Component
	{
	public:
		float
			mass = 1,
			drag = 0,
			angularDrag = 0,
			gravityScale = 1;

	};

	class Renderer : public Component
	{
	public:
		vector<Vector3> vertices;

	};
}