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

        int FPS = 0;
        int frameCount = 0;
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
        void Run();
        void Stop(EndType endType);
        void Stop(EndType endType, const char* addMessage);

    private:
        bool isRunning = false;

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void PrintLog(EndType endType);
        void GameLoops();
    };
}
