#pragma once
#include "Tools.h"
#include <chrono>

using namespace std::chrono;
using namespace GamEncin::InputSystem;
using namespace GamEncin::ToolKit;

namespace GamEncin
{
    class Scene;

    class Application //singleton
    {
    public:
        static vector<Scene*> scenes;
        static Scene* currentScene;
        static Input* input;

        static int FPS, //the number of frames that have passed in the last second
            frameCount, //the number of frames that have passed since the last second
            fixedFPS; //the frame rate of the fixed update

        static float fixedDeltaTime, //interval between fixed updates in seconds
            deltaTime, //the duration of the last frame in seconds
            accumulatedTime, //the time that has passed since the last fixed update in seconds
            secondsPastFromStart; //the time that has passed since the start of the program in seconds

        static bool printFPS, //whether to print the FPS to the console
            isRunning; //whether the program is running

        static Scene& CreateScene();
        static Scene& CreateAndUseScene();
        static void SetCurrentScene(Scene& scene);
        static void Run();
        static void PrintLog(EndType endType);
        static void PrintLog(EndType endType, const char* addMessage);
        static void Stop(EndType endType);
        static void Stop(EndType endType, const char* addMessage);

    private:
        Application() = delete;
        Application(const Application&) = delete;
        void operator=(const Application&) = delete;

        static void Awake();
        static void Start();
        static void Update();
        static void LateUpdate();
        static void FixUpdate();
        static void GameLoops();
    };
}
