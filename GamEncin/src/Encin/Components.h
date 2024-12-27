#pragma once
#include "SceneManager.h"
#include "Tools.h"
#include <iostream>
#include <string>

using std::string;
using std::vector;

namespace GamEncin
{
	class Component
	{
	public:
		virtual void Awake() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void FixUpdate() {}
	};

	class Object : public Component
	{
	public:
		Layer layer = Default;
		string name = "Object";
		Vector3
			position,
			rotation,
			scale = Vector3::One();

		Object() = default;
		~Object() = default;

		void Awake() override
		{

		}

		void Start() override
		{
			std::cout << "Object Start\n";
		}

		void Update() override
		{

		}

		void FixUpdate() override
		{

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

		void Awake() override
		{

		}

		void Start() override
		{
			std::cout << "PsychicsBody Start\n";
		}

		void Update() override
		{

		}

		void FixUpdate() override
		{

		}
	};

	class Renderer : Component
	{
	public:
		vector<Vector3> vertices;

		void Awake() override
		{

		}

		void Start() override
		{
			std::cout << "Renderer Start\n";		
			SceneManager::GetInstance().SendVerticesDataToBuffer(vertices);
		}

		void Update() override
		{

		}

		void FixUpdate() override
		{

		}
	};
}