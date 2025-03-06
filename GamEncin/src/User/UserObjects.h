#pragma once 
#include "GamEncin.h"

class TestObject : public Object
{
public:
    TestObject()
    {

    }

    void Start() override
    {
        //scale = Vector3(0.5, 1.6, 1);
        rotation = Vector3(45, 45, 45);
    }

    void FixUpdate() override
    {

    }

    void Update() override
    {
        position += Vector3(1, 1, 0) * Application::instance->deltaTime * 10;
        rotation += Vector3(1, 1, 1) * Application::instance->deltaTime;
    }
};
