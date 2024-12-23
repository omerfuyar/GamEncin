#pragma once
#include "Tools.h"
#include <iostream>
#include <string>
using std::string;

namespace GamEncin
{
	class Object
	{
	public:
		Layer layer = Default;
		string name = "Object";
		Vector3 position;
		Vector3 rotation;
		Vector3 scale = Vector3::one;

		Object() = default;
		virtual ~Object() = default;

		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void FixUpdate() {}
	};

	class PsychicsBody : public Object
	{
		float mass = 1;
		float drag = 0;
		float angularDrag = 0;
		float gravityScale = 1;

		const float GRAVITY = 9.80665f;
	};
}