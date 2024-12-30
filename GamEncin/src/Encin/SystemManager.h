#pragma once
#include "Tools.h"
#include <chrono>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <thread>
#include <vector>

using namespace std::chrono;

using std::shared_ptr;
using std::unique_ptr;

namespace GamEncin
{
    class TransformManager
    {
    public:
        unordered_map<EntityID, Transform> transformComponents;

        void Awake();
        void Start();
        void Update();
        void FixUpdate();
    };

    class PsychicsBodyManager
    {
    public:
        unordered_map<EntityID, PsychicsBody> psychicsBodyComponents;

        void Awake();
        void Start();
        void Update();
        void FixUpdate();
    };

    class RendererManager
    {
    public:
        unordered_map<EntityID, Renderer> rendererComponents;

        void Awake();
        void Start();
        void Update();
        void FixUpdate();

        void RenderFrame();
        void SendVerticesDataToBuffer(vector<Vector3> vertices);
    };

    class SystemManager
    {
    public:
        TransformManager transformManager;
        PsychicsBodyManager psychicsBodyManager;
        RendererManager rendererManager;

        void Awake();
        void Start();
        void Update();
        void FixUpdate();

        void End(int exitCode);
        void GameLoops();
    };

    class Application //singleton
    {
    private:
        Application() : systemManager() {}
        ~Application() = default;

    public:
        static Application& GetInstance()
        {
            static Application instance;
            return instance;
        }

        Application(const Application&) = delete;
        void operator=(const Application&) = delete;

        SystemManager systemManager;

        const char* vertexShaderSourceCode =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

        const char* fragmentShaderSourceCode =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
            "}\n\0";

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        void InitialRender();

        void Run();
    };
}
