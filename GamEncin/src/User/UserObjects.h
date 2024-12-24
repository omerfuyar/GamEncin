#pragma once 
#include "GamEncin.h"
#include <iostream>

class TestObject : public Object
{
public:
	void Awake() override
	{
		Object::Awake();
		//name = "myObject";
		position = Vector3::one;
		Layer layer = Default;
	}

	void Start() override
	{
		Object::Start();
		std::cout << "qwe";
	}

	void Update() override
	{
		Object::Update();
	}

	void FixUpdate() override
	{
		Object::FixUpdate();
	}
};