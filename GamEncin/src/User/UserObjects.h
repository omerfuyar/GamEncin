#pragma once 
#include "GamEncin.h"

class myObject : public Simit
{
public:
    myObject() : Simit(0.5, 0.25, 21)
    {

    }

    void Update() override
    {
        rotation += Vector3(1, 2, 1) * Application::instance->deltaTime * 10;
        //rotation += Vector3(MathYaman::RandomRangeFloat(1, 10), MathYaman::RandomRangeFloat(1, 10), MathYaman::RandomRangeFloat(1, 10)) * Application::instance->deltaTime * MathYaman::RandomRangeFloat(0, 100);
    }
};
