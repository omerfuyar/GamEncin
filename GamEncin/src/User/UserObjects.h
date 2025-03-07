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

class Sphere : public Object
{
public:
    Sphere()
    {
        GenerateSphere(1.0f, 20, 20);
    }

    void GenerateSphere(float radius, int sectorCount, int stackCount)
    {
        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
        float s, t;                                     // texCoord

        float sectorStep = 2 * MathYaman::PI / sectorCount;
        float stackStep = MathYaman::PI / stackCount;
        float sectorAngle, stackAngle;

        for(int i = 0; i <= stackCount; ++i)
        {
            stackAngle = MathYaman::PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            for(int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                vertices.push_back(Vector3(x, y, z));
                vertices.push_back(Vector3(x, y, z) * 255);

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                //normals.push_back(Vector3(nx, ny, nz));

                // vertex tex coord (s, t) range between [0, 1]
                s = (float) j / sectorCount;
                t = (float) i / stackCount;
                //texCoords.push_back(Vector2(s, t));
            }
        }

        // generate CCW index list of sphere triangles
        int k1, k2;
        for(int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for(int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if(i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if(i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
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
        rotation += Vector3(MathYaman::RandomRangeInteger(1, 10), MathYaman::RandomRangeInteger(1, 10), MathYaman::RandomRangeInteger(1, 10)) * Application::instance->deltaTime * MathYaman::RandomRangeFloat(0, 100);
    }
};
