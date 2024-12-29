#pragma once
#include "Tools.h"

namespace GamEncin
{
	enum ComponentType
	{
		NullCT, ObjectCT, TransformCT, PsychicsBodyCT, RendererCT
	};

	class Component
	{
	public:
		ComponentType type;

		bool operator==(const Component& otherComponent) 
		{
			return type == otherComponent.type;
		}
	};

	class NullComponent : public Component
	{
	public:
		NullComponent() { type = NullCT; };
	};

	class Object : public Component
	{
	public:
		Object() { type = ObjectCT; };

		string name = "Object";
		string tag = "Default Tag";
		Layer layer = Default;
	};

	class Transform : public Component
	{
	public:
		Transform() { type = TransformCT; };

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
		PsychicsBody() { type = PsychicsBodyCT; };

		float
			mass = 1,
			drag = 0,
			angularDrag = 0,
			gravityScale = 1;
	};

	class Renderer : public Component
	{
	public:
		Renderer() { type = RendererCT; };

		vector<Vector3> vertices;
	};
}