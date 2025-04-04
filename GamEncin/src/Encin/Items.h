#pragma once
#include "Tools.h"

using namespace GamEncin::MathYaman;

#define POSITION_VBO_LAYOUT 0
#define COLOR_VBO_LAYOUT 1

//TODO BE CAREFUL WITH THESE IN THE VERTEX SHADER

namespace GamEncin
{
#pragma region Forward Declarations

    class VBO;
    class EBO;
    class VAO;
    class Shader;
    class Object;
    class Shape;
    class Cube;
    class Pyramid;
    class Sphere;
    class Cylinder;
    class Plane;
    class Circle;
    class Cone;
    class Simit;
    class Camera;
    class Renderer;
    class Scene;
    class Component;
    class Transform;
    class Collider;
    class RigidBody;
    class Mesh;
    class Camera;

#pragma endregion

#pragma region OpenGL Objects

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

    class Shader
    {
    public:
        Shader(const char* vertexFile, const char* fragmentFile);
        GLuint ID,
            transformMatrixVarID,
            objPositionVarID,
            objRotationVarID,
            objScaleVarID;

        void Use();
        void Delete();

    private:
        void CheckShaderErrors(GLuint shader, const char* type);
    };

#pragma endregion

#pragma region Object And Components

    class Object
    {
    public:
        Object() = default;
        Object(Scene* scene);
        Object(Scene* scene, string name, string tag);
        Object(Scene* scene, string name, string tag, Layer layer);
        ~Object();

        string name = "Object",
            tag = "Default Tag";

        Layer layer = Default;

        Scene* scene = nullptr;
        vector<Component*> components;
        Transform* transform = AddComponent<Transform>();

        template <typename T>
        T* GetComponent()
        {
            for(Component* component : components)
            {
                T* castedComponent = dynamic_cast<T*>(component);

                if(castedComponent)
                {
                    return castedComponent;
                }
            }

            Application::PrintLog(NullPointerErr, "Component couldn't found in the object");
            return nullptr;
        }

        template <typename T>
        T* AddComponent()
        {
            auto obj = std::find_if(components.begin(), components.end(), [](Component* component) { return dynamic_cast<T*>(component); });

            if(obj != components.end())
            {
                Application::PrintLog(ElementDuplicationErr, "Component trying to add is already in the object");
                return nullptr;
            }

            T* component = new T(this);
            Component* castedComponent = dynamic_cast<Component*>(component);

            if(castedComponent)
            {
                components.push_back(component);
            }
            else
            {
                Application::Stop(TypeMismachErr, "Argument is not a Component");
            }

            return component;
        }

        template <typename T>
        void RemoveComponent()
        {
            auto obj = std::find_if(components.begin(), components.end(), [](Component* component) { return dynamic_cast<T*>(component); });

            if(obj != components.end())
            {
                components.erase(obj);
                delete* obj;
            }
            else
            {
                Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found component to remove");
            }
        }

        void RemoveComponent(Component* component);

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void StartOfSecond();
    };

    class Component
    {
    public:
        Component(Object* object);
        ~Component();

        Object* object = nullptr;

        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void FixUpdate() {}
        virtual void StartOfSecond() {}
    };

    class Transform : public Component
    {
    public:
        Transform(Object* object) : Component(object) {};

        Transform* parent = nullptr;
        vector<Transform*> children;

        Vector3 position = Vector3::Zero(),
            rotation = Vector3::Zero(),
            scale = Vector3::One(),
            localPosition = Vector3::Zero(),
            localRotation = Vector3::Zero(),
            localScale = Vector3::One(),
            direction = Vector3::Forward();

        void Update() override;

    private:
        void UpdateProperties();
    };

    class Collider : public Component
    {
        Collider(Object* object) : Component(object) {};
    };

    class RigidBody : public Component
    {
        RigidBody(Object* object) : Component(object) {};
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
        EBO* ebo = nullptr;

        void SetShape(Shape* newShape);
        void Initialize();
        void Draw();
    };

    class Camera : public Component
    {
    public:
        Camera(Object* object) : Component(object) {};
        Camera(Object* object, Vector2Int size, float FOV);
        ~Camera() = default;

        GLfloat cameraFOV = 0.0f;
        Vector2Int size = Vector2Int(1080, 1080);

        void UseCamera(GLuint& transformMatrixLocation);

    private:
        mat4 perspectiveMatrix = mat4(1.0f),
            viewMatrix = mat4(1.0f);
    };

#pragma endregion

#pragma region Shapes

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

#pragma endregion

    class Scene
    {
    public:
        Scene();

        Renderer* renderer = nullptr;
        vector<Object*> objects;

        template<typename T >
        Object* CreateObject()
        {
            Object* object = new Object(this);
            object->AddComponent<T>();
            AddObject(object);
            return object;
        }

        Object* CreateObject();
        Object* CreateObject(string name, string tag);
        Object* CreateAndUseCameraObject(Vector2Int size);
        void SetMainCamera(Camera* camera);
        void AddObject(Object* object);
        void RemoveObject(Object* object);
        void Clear();

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void StartOfSecond();
    };

    class Renderer
    {
    public:
        vector<Mesh*> meshes;
        Shader* shaderProgram = nullptr;
        Camera* mainCamera = nullptr;
        GLFWwindow* window = nullptr;

        Vector2Int initWindowSize = Vector2Int(1080, 1080);
        Vector4 clearColor = Vector4(0, 0, 0, 0);
        bool windowCloseInput = false;

        void AddMesh(Mesh* mesh);
        void RemoveMesh(Mesh* mesh);
        void RenderFrame();
        void InitialRender();
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
        void EndRenderer();

    private:
        void GLSendUniformVector3(GLuint& location, Vector3 vector3);
        void ClearColor(Vector4 clearColor);
    };
}
