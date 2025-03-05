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
    public:
        Application();

        static Application* instance;
        vector<Scene*> scenes;
        Scene* currentScene = nullptr;
        Renderer* renderer = nullptr;
        Vector2 windowSize;

        int FPS = 0;
        int fixedFPS = 0; //the frame rate of the fixed update
        float fixedDeltaTime = 0.0; //interval between fixed updates in seconds
        float deltaTime = 0.0; //the duration of the last frame in seconds
        float accumulatedTime = 0.0; //the time that has passed since the last fixed update in seconds
        float secondsPastFromStart = 0.0;
        bool printFPS = false;

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
