#pragma once
#include "Tools.h"
#include <chrono>
#include <glad.h>
#include <glfw3.h>
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

        double deltaTime = 0; //the duration of the last frame in seconds
        const int fixedFPS = 50;
        const double fixedDeltaTime = 1 / fixedFPS; //interval between fixed updates in seconds
        long long msPastFromStart = 0;
        int FPS = 0;
        bool printFPS = true;

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
        void Stop(EndType et);
    };
}
