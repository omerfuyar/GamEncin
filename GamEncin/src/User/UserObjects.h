#pragma once 
#include "GamEncin.h"

class myObject : public Pyramid
{
public:
    myObject() : Pyramid()
    {

    }

    void Update() override
    {
        //rotation += Vector3(0, 0, 1) * Application::instance->deltaTime * 100;
    }
};
