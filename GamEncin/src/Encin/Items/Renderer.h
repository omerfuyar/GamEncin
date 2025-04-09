#pragma once
#include "Encin/Tools/MathYaman.h"

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

namespace GamEncin
{
    class Camera;
    class Mesh;
    struct Shader;

    class Renderer
    {
    public:
        vector<Mesh*> meshes;
        Shader* shaderProgram = nullptr;
        Camera* mainCamera = nullptr;
        GLFWwindow* window = nullptr;

        Vector2Int initWindowSize = Vector2Int(1080, 1080);
        Vector4 clearColor = Vector4::Zero();
        bool windowCloseInput = false;

        void AddMesh(Mesh* mesh);
        void RemoveMesh(Mesh* mesh);
        void RenderFrame();
        void InitialRender();
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
        void EndRenderer();

    private:
        void GLSendUniformVector3(unsigned int& location, Vector3 vector3);
        void ClearColor(Vector4 clearColor);
    };
}
