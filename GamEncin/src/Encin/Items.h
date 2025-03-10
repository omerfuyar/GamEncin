#pragma once
#include "Tools.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#ifndef POSITION_VBO_LAYOUT
#define POSITION_VBO_LAYOUT 0
#endif // !POSITION_VBO_LAYOUT

#ifndef COLOR_VBO_LAYOUT
#define COLOR_VBO_LAYOUT 1
#endif // !COLOR_VBO_LAYOUT

//TODO BE CAREFUL WITH THESE IN VERTEX SHADER

namespace GamEncin
{
    class VAO;
    class VBO;
    class EBO;
    class Shader;
    class Renderer;

    class Object
    {
    public:
        ~Object();

        string name = "Object",
            tag = "Default Tag";

        Layer layer = Default;

        float mass = 1,
            gravityScale = 1,
            drag = 0,
            angularDrag = 0;

        Vector3 position,
            rotation,
            scale = Vector3::One();

        VAO* vao = nullptr;

        vector<Vector3> vertices;
        VBO* vbo = nullptr;

        vector<GLuint> indices;
        EBO* ebo = nullptr;

        void Draw();
        void Initialize();
        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void FixUpdate() {}
    };

#pragma region Shapes

    class Cube : public Object
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

    class Pyramid : public Object
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

    class Sphere : public Object
    {
    public:
        float radius;
        int resolution;

        Sphere(float radius = 1.0, int resolution = 20)
        {
            this->radius = radius;
            this->resolution = resolution;

            //TODO this is not compatible with shaders and normals
            //this sphere is rendering from top to bottom, 
            float x, y, z, xz;

            float stackStep = MathYaman::PI / resolution; //radian
            float sectorStep = 2 * MathYaman::PI / resolution; //radian
            float currStackAngle, currSectorAngle;

            for(int i = 0; i <= resolution; ++i)
            {
                currStackAngle = MathYaman::PI / 2 - i * stackStep; //from pi/2 to -pi/2 inclusive

                xz = radius * MathYaman::CosRad(currStackAngle);
                y = radius * MathYaman::SinRad(currStackAngle);

                for(int j = 0; j <= resolution; ++j)
                {
                    currSectorAngle = j * sectorStep; //from 0 to 2pi inclusive

                    x = xz * MathYaman::CosRad(currSectorAngle);
                    z = xz * MathYaman::SinRad(currSectorAngle);

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

    class Cylinder : public Object
    {
    public:
        float height, radius;
        int resolution;

        Cylinder(float height = 1.0, float radius = 0.5, int resolution = 20)
        {
            this->height = height;
            this->radius = radius;
            this->resolution = resolution;

            float x, y, z;
            float sectorStep = 2 * MathYaman::PI / resolution; //radian
            float currSectorAngle;

            for(int i = -1; i < 2; i += 2)
            {
                y = i * height / 2;
                vertices.push_back(Vector3(0, y, 0));
                //vertices.push_back(Vector3(255, 16, 240));
                vertices.push_back(Vector3(0, y, 0) * 255);

                for(int j = 0; j < resolution; ++j)
                {
                    currSectorAngle = j * sectorStep; //from 0 to 2pi inclusive

                    x = radius * MathYaman::CosRad(currSectorAngle);
                    z = radius * MathYaman::SinRad(currSectorAngle);

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
                indices.push_back(center2);
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

    class Plane : public Object
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

    class Circle : public Object
    {
    public:
        float radius;
        int resolution;

        Circle(float radius = 1.0, int resolution = 20)
        {
            this->radius = radius;
            this->resolution = resolution;

            float x, z;
            float sectorStep = 2 * MathYaman::PI / resolution; //radian
            float currSectorAngle;

            vertices.push_back(Vector3(0, 0, 0));
            //vertices.push_back(Vector3(255, 16, 240));
            vertices.push_back(Vector3(0, 0, 0) * 255);

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep; //from 0 to 2pi inclusive

                x = radius * MathYaman::CosRad(currSectorAngle);
                z = radius * MathYaman::SinRad(currSectorAngle);

                vertices.push_back(Vector3(x, 0, z));
                //vertices.push_back(Vector3(255, 16, 240) * (j % 2));
                vertices.push_back(Vector3(x, 0, z) * 255);
            }

            for(int i = 0; i < resolution; i++)
            {
                indices.push_back(0); //center1
                indices.push_back(i + 1);
                indices.push_back((i + 1) % resolution + 1);
            }
        }
    };

    class Cone : public Object
    {
    public:
        float height, radius;
        int resolution;

        Cone(float height = 1.0, float radius = 1.0, int resolution = 20)
        {
            this->height = height;
            this->radius = radius;
            this->resolution = resolution;

            float x, z;
            float yQuarter = height / 4;
            float sectorStep = 2 * MathYaman::PI / resolution; //radian
            float currSectorAngle;

            vertices.push_back(Vector3(0, -yQuarter, 0));
            vertices.push_back(Vector3(0, -yQuarter, 0) * 255);

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep; //from 0 to 2pi inclusive

                x = radius * MathYaman::CosRad(currSectorAngle);
                z = radius * MathYaman::SinRad(currSectorAngle);

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

    class Simit : public Object
    {
    public:
        float radius, thickness;
        int resolution;

        Simit(float radius = 1.0, float thickness = 0.2, int resolution = 20)
        {
            this->radius = radius;
            this->thickness = thickness;
            this->resolution = resolution;

            float x, y, z;
            float sectorStep = 2 * MathYaman::PI / resolution; //radian
            float currSectorAngle;

            for(int i = 0; i < resolution; ++i)
            {
                currSectorAngle = i * sectorStep; //from 0 to 2pi inclusive

                x = radius * MathYaman::CosRad(currSectorAngle);
                z = radius * MathYaman::SinRad(currSectorAngle);

                vertices.push_back(Vector3(x, y, z));
                //vertices.push_back(Vector3(255, 16, 240) * (j % 2));
                vertices.push_back(Vector3(x, y, z) * 255);
            }

            for(int i = 0; i < resolution; i++)
            {

            }

        }
    };

#pragma endregion

    class Scene
    {
    public:
        Scene();

        vector<Object*> objects;

        template <typename T>
        T* CreateObject()
        {
            T* object = new T();

            Object* castedObject = dynamic_cast<Object*>(object);

            if(!castedObject)
            {
                Application::instance->Stop(TypeMismachErr);
            }

            objects.push_back(castedObject);

            return object;
        }

        void AddObject(Object* object);
        void RemoveObject(Object* object);
        void Clear();
        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
    };

    class Shader //Shader Program Object
    {
    public:
        Shader(const char* vertexFile, const char* fragmentFile);
        GLuint ID, positionDividerVarID, positionVarID, rotationVarID, scaleVarID;

        void CheckShaderErrors(GLuint shader, const char* type);
        void Use();
        void Delete();
    };

    class VBO
    {
    public:
        VBO(vector<Vector3> vertices);
        GLuint ID;

        void Bind();
        void Update(vector<Vector3> vertices);
        void Delete();
    };

    class EBO
    {
    public:
        EBO(vector<GLuint> indices);
        GLuint ID;

        void Bind();
        void Delete();
    };

    class VAO
    {
    public:
        VAO();
        GLuint ID;

        void LinkAttributes(GLuint layout, GLuint numComponents, GLenum type, GLuint offsetInBytes);
        void Bind();
        void Delete();
    };

    class Renderer
    {
    public:
        Shader* shaderProgram = nullptr;
        GLFWwindow* window = nullptr;
        Vector4 clearColor = Vector4(0, 0, 0, 0);
        GLfloat positionDivider = 0.0;
        bool windowCloseInput = false;

        void RenderFrame(vector<Object*> objects);
        void InitialRender(vector<Object*> objects);
        void ClearColor(Vector4 clearColor);
        void GLSendUniformVector3(GLuint location, Vector3 vector);
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
        void EndRenderer();
    };
}
