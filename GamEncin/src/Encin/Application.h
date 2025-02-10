#pragma once
#include "Tools.h"
#include <chrono>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

using namespace std::chrono;

namespace GamEncin
{
    class Scene;

    class Application //singleton
    {
    private:
        Application() {};
        ~Application() = default;

    public:
        vector<Scene*> scenes;
        Scene* currentScene = nullptr;

        const int FPSlimit = 0; // 0 : no limit
        int fixedFPS = 50;
        bool printFPS = false;

        static Application& GetInstance()
        {
            static Application instance;
            return instance;
        }

        Application(const Application&) = delete;
        void operator=(const Application&) = delete;

        Scene& CreateScene();
        Scene& CreateAndUseScene();
        void SetCurrentScene(Scene& scene);
        void Awake();
        void Start();
        void Update();
        void FixUpdate();
        void RenderFrame();
        void Run();
        void InitialRender();
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        void GameLoops();
        void End(EndType et);
    };
}
