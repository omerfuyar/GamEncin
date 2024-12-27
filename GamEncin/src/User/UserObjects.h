#pragma once 
#include "GamEncin.h"

class TestObject : public Object, Renderer, PsychicsBody
{
public:
	void Awake() override
	{
		Object::Awake();
		PsychicsBody::Awake();
		Renderer::Awake();

		name = "Test Object";
		position = Vector3::Zero();
		vertices = {
			Vector3(2, 1, 0),
			Vector3(0.5, -0.5, 0),
			Vector3(0, 0.5, 0)
		};
	}

	void Start() override
	{
		Object::Start();
		PsychicsBody::Start();
		Renderer::Start();

		std::cout << "qwe	";
	}

	void Update() override
	{
		Object::Update();
		PsychicsBody::Update();
		Renderer::Update();
	}

	void FixUpdate() override
	{
		Object::FixUpdate();
		PsychicsBody::FixUpdate();
		Renderer::FixUpdate();
	}
};