#pragma once
#include "Tools.h"
#include <chrono>

using namespace std::chrono;

namespace GamEncin
{
    class Scene;
    class Renderer;

    class Application //singleton
    {
    private:
        Application() {};
        ~Application() = default;

    public:
        vector<Scene*> scenes;
        Scene* currentScene = nullptr;
        Renderer* renderer = nullptr;

        int FPS = 0;
        int fixedFPS = 50;
        float fixedDeltaTime = 1.0f / (float) fixedFPS; //interval between fixed updates in seconds
        float deltaTime = 0.0f; //the duration of the last frame in seconds
        float accumulatedTime = 0.0f;
        float secondsPastFromStart = 0.0f;
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
        void Run();
        void GameLoops();
        void Stop(EndType et);
    };
}
