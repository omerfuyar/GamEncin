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
	public:
		float mass = 1,
			drag = 0,
			angularDrag = 0,
			gravityScale = 1;



	};
}