#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Items/OpenGLObjects.h"
#include "Encin/Tools/MathYaman.h"

#include <GLAD/glad.h>

namespace GamEncin
{
    //TODO fix this
    class Shape
    {
    public:
        vector<GLuint> indices;
        vector<Vector3> vertices;
    };

    class Cube : public Shape
    {
    public:
        float sideLength;

        Cube(float sideLength = 1.0)
        {
            this->sideLength = sideLength;
            float xyzCoord = sideLength / 2;

            vertices =
            {
                Vector3(-1, -1, -1) * xyzCoord, Vector3(128, 0, 128),
                Vector3(1, -1, -1) * xyzCoord, Vector3(255, 165, 0),
                Vector3(1, -1, 1) * xyzCoord, Vector3(0, 128, 128),
                Vector3(-1, -1, 1) * xyzCoord, Vector3(75, 0, 130),
                Vector3(-1, 1, 1) * xyzCoord, Vector3(255, 20, 147),
                Vector3(1, 1, 1) * xyzCoord, Vector3(0, 255, 127),
                Vector3(1, 1, -1) * xyzCoord, Vector3(255, 105, 180),
                Vector3(-1, 1, -1) * xyzCoord, Vector3(0, 191, 255)
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
    };

    class Pyramid : public Shape
    {
    public:
        float height, baseLength;

        Pyramid(float height = 1.0, float baseLength = 1.0)
        {
            this->height = height;
            this->baseLength = baseLength;

            float xzCoord = baseLength / 2;
            float yQuarter = height / 4;

            vertices =
            {
                Vector3(0.0, yQuarter * 3, 0.0), Vector3(255, 0, 0),
                Vector3(-xzCoord, -yQuarter, xzCoord), Vector3(0, 255, 0),
                Vector3(xzCoord, -yQuarter, xzCoord), Vector3(0, 0, 255),
                Vector3(xzCoord, -yQuarter, -xzCoord), Vector3(255, 255, 0),
                Vector3(-xzCoord, -yQuarter, -xzCoord), Vector3(0, 255, 255)
            };

            indices = {
                0, 1, 2, 0, 2, 3,
                0, 3, 4, 0, 4, 1,
                1, 2, 3, 3, 4, 1
            };
        }
    };

    class Sphere : public Shape
    {
    public:
        float halfRadius;
        int resolution;

        Sphere(float halfRadius = 0.5, int resolution = 20)
        {
            this->halfRadius = halfRadius;
            this->resolution = resolution;

            //TODO this is not compatible with shaders and normals
            //this sphere is rendering from top to bottom, 
            float x, y, z, xz;
            float stackStep = PI / resolution; //radian
            float sectorStep = 2 * PI / resolution; //radian
            float currStackAngle, currSectorAngle;

            for(int i = 0; i <= resolution; ++i)
            {
                currStackAngle = PI / 2 - i * stackStep; //from pi/2 to -pi/2 inclusive

                xz = halfRadius * CosRad(currStackAngle);
                y = halfRadius * SinRad(currStackAngle);

                for(int j = 0; j <= resolution; ++j)
                {
                    currSectorAngle = j * sectorStep; //from 0 to 2pi inclusive

                    x = xz * CosRad(currSectorAngle);
                    z = xz * SinRad(currSectorAngle);

                    vertices.push_back(Vector3(x, y, z)); //for position
                    //vertices.push_back(Vector3(255, 16, 240) * (j % 2)); //for color
                    vertices.push_back(Vector3(x, y, z) * 255); //for color
                }
            }

            // generate CCW index list of sphere triangles
            int k1, k2;
            for(int i = 0; i < resolution; ++i)
            {
                k1 = i * (resolution + 1);     // beginning of current stack
                k2 = k1 + resolution + 1;      // beginning of next stack

                for(int j = 0; j < resolution; ++j, ++k1, ++k2)
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
                    if(i != (resolution - 1))
                    {
                        indices.push_back(k1 + 1);
                        indices.push_back(k2);
                        indices.push_back(k2 + 1);
                    }
                }
            }
        }
    };

    class Cylinder : public Shape
    {
    public:
        float height, halfRadius;
        int resolution;

        Cylinder(float height = 1.0, float halfRadius = 0.5, int resolution = 20)
        {
            this->height = height;
            this->halfRadius = halfRadius;
            this->resolution = resolution;

            float x, y, z;
            float sectorStep = 2 * PI / resolution; //radian
            float currSectorAngle;

            for(int i = -1; i < 2; i += 2)
            {
                y = i * height / 2;
                vertices.push_back(Vector3(0, y, 0));
                //vertices.push_back(Vector3(255, 16, 240));
                vertices.push_back(Vector3(0, y, 0) * 255);

                for(int j = 0; j < resolution; ++j)
                {
                    currSectorAngle = j * sectorStep;

                    x = halfRadius * CosRad(currSectorAngle);
                    z = halfRadius * SinRad(currSectorAngle);

                    vertices.push_back(Vector3(x, y, z));
                    //vertices.push_back(Vector3(255, 16, 240) * (j % 2));
                    vertices.push_back(Vector3(x, y, z) * 255);
                }
            }

            int center2 = resolution + 1;
            for(int i = 0; i < resolution; ++i)
            {
                //circles
                indices.push_back(0); //center1
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(center2); //center2
                indices.push_back(center2 + i + 1);
                indices.push_back(center2 + (i + 1) % resolution + 1);

                //sides
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(i + resolution + 2);
                indices.push_back((i + 1) % resolution + 1);
                indices.push_back(i + resolution + 2);
                indices.push_back((i + 1) % resolution + resolution + 2);
            }
        }
    };

    class Plane : public Shape
    {
    public:
        float sideLength;

        Plane(float sideLength = 1.0)
        {
            this->sideLength = sideLength;

            float xzCoord = sideLength / 2;

            vertices =
            {
                Vector3(1, 0, 1) * xzCoord, Vector3(1, 0, 1) * xzCoord * 255,
                Vector3(1, 0, -1) * xzCoord, Vector3(1, 0, -1) * xzCoord * 255,
                Vector3(-1, 0, -1) * xzCoord, Vector3(-1, 0, -1) * xzCoord * 255,
                Vector3(-1, 0, 1) * xzCoord, Vector3(-1, 0, 1) * xzCoord * 255
            };

            indices = {
                0, 1, 2, 2, 3, 0
            };
        }
    };

    class Circle : public Shape
    {
    public:
        float halfRadius;
        int resolution;

        Circle(float halfRadius = 0.5, int resolution = 20)
        {
            this->halfRadius = halfRadius;
            this->resolution = resolution;

            float x, z;
            float sectorStep = 2 * PI / resolution; //radian
            float currSectorAngle;

            vertices.push_back(Vector3(0, 0, 0));
            //vertices.push_back(Vector3(255, 16, 240));
            vertices.push_back(Vector3(0, 0, 0) * 255);

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep;

                x = halfRadius * CosRad(currSectorAngle);
                z = halfRadius * SinRad(currSectorAngle);

                vertices.push_back(Vector3(x, 0, z));
                //vertices.push_back(Vector3(255, 16, 240) * (j % 2));
                vertices.push_back(Vector3(x, 0, z) * 255);
            }

            for(int i = 0; i < resolution; i++)
            {
                indices.push_back(0); //center
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
            }
        }
    };

    class Cone : public Shape
    {
    public:
        float height, halfRadius;
        int resolution;

        Cone(float height = 1.0, float halfRadius = 0.5, int resolution = 20)
        {
            this->height = height;
            this->halfRadius = halfRadius;
            this->resolution = resolution;

            float x, z;
            float yQuarter = height / 4;
            float sectorStep = 2 * PI / resolution; //radian
            float currSectorAngle;

            vertices.push_back(Vector3(0, -yQuarter, 0));
            vertices.push_back(Vector3(0, -yQuarter, 0) * 255);

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep;

                x = halfRadius * CosRad(currSectorAngle);
                z = halfRadius * SinRad(currSectorAngle);

                vertices.push_back(Vector3(x, -yQuarter, z));
                //vertices.push_back(Vector3(255, 16, 240) * (j % 2));
                vertices.push_back(Vector3(x, -yQuarter, z) * 255);
            }

            vertices.push_back(Vector3(0, yQuarter * 3, 0));
            vertices.push_back(Vector3(0, yQuarter * 3, 0) * 255);

            for(int i = 0; i < resolution; i++)
            {
                //circle
                indices.push_back(0); //center1
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);

                //sides
                indices.push_back(resolution + 1); //top
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
            }
        }
    };

    class Simit : public Shape
    {
    public:
        float halfRadius, halfThickness;
        int resolution;

        Simit(float halfRadius = 0.5, float halfThickness = 0.25, int resolution = 20)
        {
            this->halfRadius = halfRadius;
            this->halfThickness = halfThickness;
            this->resolution = resolution;

            float x, y, z;
            float sectorSteps = 2 * PI / resolution; //radian
            float currMajorAngle, currMinorAngle;

            for(int i = 0; i < resolution; ++i)
            {
                currMajorAngle = i * sectorSteps;

                for(int j = 0; j < resolution; j++)
                {
                    currMinorAngle = j * sectorSteps;
                    x = (halfRadius + halfThickness * CosRad(currMinorAngle)) * CosRad(currMajorAngle);
                    y = halfThickness * SinRad(currMinorAngle);
                    z = (halfRadius + halfThickness * CosRad(currMinorAngle)) * SinRad(currMajorAngle);

                    vertices.push_back(Vector3(x, y, z));
                    //vertices.push_back(Vector3(255, 16, 240) * (j % 2));
                    vertices.push_back(Vector3(x, y, z) * 255);
                }
            }

            for(int i = 0; i < resolution; i++) //major
            {
                int currMajorIndex = (i % resolution) * resolution;
                int nextMajorIndex = ((i + 1) % resolution) * resolution;

                for(int j = 0; j < resolution; j++) //minor
                {
                    indices.push_back(currMajorIndex + j);
                    indices.push_back(nextMajorIndex + j);
                    indices.push_back(currMajorIndex + (j + 1) % resolution);
                    indices.push_back(nextMajorIndex + j);
                    indices.push_back(nextMajorIndex + (j + 1) % resolution);
                    indices.push_back(currMajorIndex + (j + 1) % resolution);
                }
            }

        }
    };

    struct Edge;
    struct Face;

    struct Vertex
    {
        unsigned int id = 0;

        Vector3 position = Vector3::Zero();
        Vector3 normal = Vector3::Forward();

        Vector2 texture = Vector2::Zero();
        Vector4 color = Vector4::One();

        vector<Face*> faces;
        vector<Edge*> edges;

        void SetID(unsigned int id);
        void SetPosition(Vector3 position);
        void SetNormal(Vector3 normal);
        void SetTexture(Vector2 texture);
        void SetColor(Vector4 color);

        void AddFace(Face* face);
        void AddEdge(Edge* edge);
    };

    struct Edge
    {
        unsigned int id = 0;

        Vertex* startVertex = nullptr;
        Vertex* endVertex = nullptr;

        Face* leftFace = nullptr;
        Face* rightFace = nullptr;

        Edge* nextLeftEdge = nullptr;
        Edge* nextRightEdge = nullptr;
        Edge* prevLeftEdge = nullptr;
        Edge* prevRightEdge = nullptr;

        void SetID(unsigned int id);
    };

    struct Face
    {
        unsigned int id = 0;

        array<Vertex*, 3> vertices;
        array<Edge*, 3> edges;

        void SetID(unsigned int id);
    };

    struct MeshData
    {
        vector<Vertex*> vertices;
        vector<Edge*> edges;
        vector<Face*> faces;
    };

    class Mesh : public Component
    {
    public:
        Mesh(Object* object);
        ~Mesh();

        vector<GLuint> indices;
        vector<Vector3> vertices;
        VAO* vao = nullptr;
        VBO* vbo = nullptr;
        IBO* ebo = nullptr;

        void SetShape(Shape* newShape);
        void Initialize();
        void Draw();
    };
}
