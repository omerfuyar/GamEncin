#pragma once
#include "Tools.h"
#include <chrono>
#include <glad.h>
#include <glfw3.h>

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

        int FPS = 0;
        int fixedFPS = 50;
        float fixedDeltaTime = 1.0 / (float) fixedFPS; //interval between fixed updates in seconds
        float deltaTime = 0.0; //the duration of the last frame in seconds
        float accumulatedTime = 0.0;
        float secondsPastFromStart = 0.0;
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
