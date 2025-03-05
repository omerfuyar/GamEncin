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
        ~Application() = default;

    public:
        Application();

        static Application* instance;
        vector<Scene*> scenes;
        Scene* currentScene = nullptr;
        Renderer* renderer = nullptr;
        Vector2 windowSize;

        int FPS;
        int fixedFPS; //the frame rate of the fixed update
        float fixedDeltaTime; //interval between fixed updates in seconds
        float deltaTime; //the duration of the last frame in seconds
        float accumulatedTime; //the time that has passed since the last fixed update in seconds
        float secondsPastFromStart;
        bool printFPS = true;

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
