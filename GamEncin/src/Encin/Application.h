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
        static string programName;

        static int fixedFPS; //the frame rate of the fixed update

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
        static void PrintLog(LogType endType, string addMessage);
        static void Stop(LogType endType);
        static void Stop(LogType endType, string addMessage);

    private:
        Application() = delete;
        Application(const Application&) = delete;
        void operator=(const Application&) = delete;

        static void Awake();
        static void Start();
        static void Update();
        static void LateUpdate();
        static void FixUpdate();
        static void StartOfSecond();
        static void GameLoops();
        static void PrintLog(LogType endType);
    };
}
