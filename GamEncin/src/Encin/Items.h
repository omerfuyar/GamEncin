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
    class VAO;
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

        VAO* vao = nullptr;

        vector<Vector3> vertices;
        GLfloat* verticeArr = new GLfloat[9];
        VBO* vbo = nullptr;

        vector<GLuint> indices;
        GLuint* indicesArr = new GLuint[3];
        EBO* ebo = nullptr;

        void SetVerticeArr(vector<Vector3> vertices, GLfloat* targetArr);
        void SetIndicesArr(vector<GLuint> indices, GLuint* targetArr);
        void Draw();
        void Initialize();
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
        void Update(GLfloat* vertices, GLsizeiptr size);
        void Delete();
    };

    class EBO
    {
    public:
        EBO(GLuint* indices, GLsizeiptr size);
        GLuint ID;

        void Bind();
        void Delete();
    };

    class VAO
    {
    public:
        VAO();
        GLuint ID;

        void LinkAttirbutes(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Delete();
    };

    class Renderer
    {
    public:
        Shader* shaderProgram = nullptr;
        GLFWwindow* window = nullptr;
        bool windowCloseInput = false;

        void RenderFrame(vector<Object*> objects);
        void InitialRender(vector<Object*> objects);
        static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
        void EndRenderer();
    };
}
