#pragma once
#include "Tools.h"

namespace GamEncin
{
    class Object
    {
    public:
        Object() {}
        virtual ~Object() {}

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

        vector<Vector3> vertices;

        void SendVerticesDataToBuffer(vector<Vector3> vertices);
        virtual void Awake() {};
        virtual void Start() {};
        virtual void Update() {};
        virtual void FixUpdate() {};
    };

    class Scene
    {
    public:
        Scene();

        vector<Object*> objects;
        Renderer* renderer = nullptr;

        template <typename T>
        T& CreateObject()
        {
            T* object = new T();
            objects.push_back(dynamic_cast<Object*>(object));
            return *object;
        }

        void AddObject(Object& object);
        void RemoveObject(Object& object);
        void Clear();
        void Awake();
        void Start();
        void Update();
        void FixUpdate();
    };

    class Renderer
    {
    public:
        GLFWwindow* window;
        GLuint shaderProgram, VBO, VAO; // Vertex Buffer Object, Vertex Array Object

        const char* vertexShaderSourceCode =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }";

        const char* fragmentShaderSourceCode =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{ FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f); }";

        void RenderFrame();
        void InitialRender();
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };
}
