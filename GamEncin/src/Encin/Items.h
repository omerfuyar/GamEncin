#pragma once
#include "Tools.h"
#include <glad.h>
#include <glfw3.h>
#include <glm.hpp>

#ifndef POSITION_VBO_LAYOUT
#define POSITION_VBO_LAYOUT 0
#endif // !POSITION_VBO_LAYOUT

#ifndef COLOR_VBO_LAYOUT
#define COLOR_VBO_LAYOUT 1
#endif // !COLOR_VBO_LAYOUT

//BE CAREFUL WITH THESE IN VERTEX SHADER

namespace GamEncin
{
    class VBO;
    class EBO;

    class Object
    {
    public:
        Object() {};
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

        vector<Vector3> vertices;
        GLfloat verticeArr[9];
        VBO* positionVBO = nullptr;

        GLuint indicesArr[3] = {0, 1, 2};
        EBO* positionEBO = nullptr;

        void SetVerticeArr(vector<Vector3> vertices, GLfloat(&targetArr)[9]);
        void SendDataToBuffer();
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

    class Shader //Shader Program Object
    {
    public:
        Shader(const char* vertexFile, const char* fragmentFile);
        GLuint ID;

        void Use();
        void Delete();
    };

    class VBO
    {
    public:
        VBO(GLfloat* vertices, GLsizeiptr size);
        GLuint ID;

        void Bind();
        void Unbind();
        void Delete();
    };

    class EBO
    {
    public:
        EBO(GLuint* indices, GLsizeiptr size);
        GLuint ID;

        void Bind();
        void Unbind();
        void Delete();
    };

    class VAO
    {
    public:
        VAO();
        GLuint ID;

        void LinkAttirbutes(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Unbind();
        void Delete();
    };

    class Renderer
    {
    public:
        Shader* shaderProgram = nullptr;
        VAO* mainVAO = nullptr;
        GLFWwindow* window = nullptr;
        bool windowCloseInput = false;

        void RenderFrame();
        void InitialRender();
        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
        void EndRenderer();
    };
}
