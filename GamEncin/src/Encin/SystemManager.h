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
    class Manager
    {
    public:

        virtual void UpdateManager() {}
    };

    class TransformManager : public Manager
    {
    public:
        unordered_map<EntityID, Transform*> transformComponents;

        void UpdateManager() override;
    };

    class PsychicsBodyManager : public Manager
    {
    public:
        unordered_map<EntityID, PsychicsBody*> psychicsBodyComponents;

        void UpdateManager() override;
    };

    class RendererManager : public Manager
    {
    public:
        unordered_map<EntityID, Renderer*> rendererComponents;

        void UpdateManager() override;

        void RenderFrame();
        void SendVerticesDataToBuffer(vector<Vector3> vertices);
    };

    class SystemManager
    {
    public:
        TransformManager transformManager;
        PsychicsBodyManager psychicsBodyManager;
        RendererManager rendererManager;

        vector<Manager> managers;

        void Awake();
        void Start();
        void Update();
        void FixUpdate();

        void GameLoops();
    };

    class Application //singleton
    {
    private:
        Application() : systemManager() {}
        ~Application() = default;

    public:
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

        static Application& GetInstance()
        {
            static Application instance;
            return instance;
        }

        Application(const Application&) = delete;
        void operator=(const Application&) = delete;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        void InitialRender();

        void Run();
        void End(int exitCode);
    };
}
