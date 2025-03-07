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
            Vector3(-0.5, -0.5, -0.5), Vector3(128, 0, 128),   // Purple
            Vector3(0.5, -0.5, -0.5), Vector3(255, 165, 0),    // Orange
            Vector3(0.5, -0.5, 0.5), Vector3(0, 128, 128),     // Teal
            Vector3(-0.5, -0.5, 0.5), Vector3(75, 0, 130),     // Indigo
            Vector3(-0.5, 0.5, 0.5), Vector3(255, 20, 147),    // Deep Pink
            Vector3(0.5, 0.5, 0.5), Vector3(0, 255, 127),      // Spring Green
            Vector3(0.5, 0.5, -0.5), Vector3(255, 105, 180),   // Hot Pink
            Vector3(-0.5, 0.5, -0.5), Vector3(0, 191, 255)     // Deep Sky Blue
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
        //scale /= 5;
    }

    void FixUpdate() override
    {

    }

    void Update() override
    {
        rotation += Vector3(1, 1, 1) * Application::instance->deltaTime * 100;
    }
};

class Pyramid : public Object
{
public:
    Pyramid()
    {
        vertices =
        {
            // Position                // Color
            Vector3(0.0, 0.5, 0.0), Vector3(255, 0, 0),       // Top (Red)
            Vector3(-0.5, -0.5, 0.5), Vector3(0, 255, 0),     // Front-left (Green)
            Vector3(0.5, -0.5, 0.5), Vector3(0, 0, 255),      // Front-right (Blue)
            Vector3(0.5, -0.5, -0.5), Vector3(255, 255, 0),   // Back-right (Yellow)
            Vector3(-0.5, -0.5, -0.5), Vector3(0, 255, 255)   // Back-left (Cyan)
        };

        indices = {
            0, 1, 2,  // Front face
            0, 2, 3,  // Right face
            0, 3, 4,  // Back face
            0, 4, 1,  // Left face
            1, 2, 3, 3, 4, 1  // Bottom face
        };
    }

    void Awake() override
    {

    }

    void Start() override
    {
        //scale /= 5;
    }

    void FixUpdate() override
    {

    }

    void Update() override
    {
        rotation += Vector3(1, 1, 1) * Application::instance->deltaTime * MathYaman::RandomRangeFloat(0, 100);
    }
};
