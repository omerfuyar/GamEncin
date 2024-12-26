#pragma once 
#include "GamEncin.h"
#include <iostream>

using namespace GamEncin;

class TestObject : public GamEncin::Object, public GamEncin::Renderer
{
public:
	void Awake() override
	{
		//name = "myObject";
		position = Vector3::one;
		Layer layer = Default;
		vertices = {
			Vector3(-0.5f,-0.5f,0),
			Vector3(0.5f,-0.5f,0),
			Vector3(0,0.5f,0)
		};
	}

	void Start() override
	{
		std::cout << "qwe";
	}

	void Update() override
	{
	}

	void FixUpdate() override
	{
	}
};