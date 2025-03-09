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
        virtual void Awake() {};
        virtual void Start() {};
        virtual void Update() {};
        virtual void LateUpdate();
        virtual void FixUpdate() {};
    };

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
