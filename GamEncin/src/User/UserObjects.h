#pragma once 
#include "GamEncin.h"

class Cube : public Object
{
public:
    Cube()
    {
        vertices =
        {
            // Position                // Color
            Vector3(-0.5, -0.5, -0.5), Vector3(255, 0, 0),
            Vector3(0.5, -0.5, -0.5), Vector3(0, 255, 0),
            Vector3(0.5, -0.5, 0.5), Vector3(0, 0, 255),
            Vector3(-0.5, -0.5, 0.5), Vector3(255, 255, 0),
            Vector3(-0.5, 0.5, 0.5), Vector3(255, 0, 255),
            Vector3(0.5, 0.5, 0.5), Vector3(0, 255, 255),
            Vector3(0.5, 0.5, -0.5), Vector3(255, 255, 255),
            Vector3(-0.5, 0.5, -0.5), Vector3(0, 0, 0)

        };

        indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            1, 2, 5, 5, 6, 1,
            0, 1, 6, 6, 7, 0,
            2, 3, 4, 4, 5, 2,
            3, 0, 7, 7, 4, 3
        };
    }

    void Awake() override
    {

    }

    void Start() override
    {
        scale /= 10;
    }

    void FixUpdate() override
    {

    }

    void Update() override
    {
        //rotation += Vector3(1, 1, 1) * Application::instance->deltaTime;
    }
};
