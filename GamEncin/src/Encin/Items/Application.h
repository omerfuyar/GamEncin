#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

#include <chrono>

using namespace std::chrono;

namespace GamEncin
{
    class Scene;

    class Application //singleton
    {
    public:
        static float fixedDeltaTime, //interval between fixed updates in seconds
            deltaTime, //the duration of the last frame in seconds
            accumulatedTime, //the time that has passed since the last fixed update in seconds
            secondsPastFromStart; //the time that has passed since the start of the program in seconds

        static bool printFPS, //whether to print the FPS to the console
            isRunning; //whether the program is running

        static void SetCurrentScene(Scene* scene);
        static void SetFixedFPS(int fps);
        static void SetProgramName(string name);
        static void SetFPSPrint(bool printFPS);

        static Scene* GetCurrentScene();
        static GLFWwindow* GetMainWindow();
        static string GetProgramName();

        static Scene& CreateScene();
        static Scene& CreateAndUseScene();
        static void Run();
        static void Restart();
        static void PrintLog(LogType endType, string addMessage);
        static void Stop(LogType endType);
        static void Stop(LogType endType, string addMessage);

    private:
        static vector<Scene*> scenes;
        static Scene* currentScene;
        static string programName;
        static GLFWwindow* window;

        static int fixedFPS;

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
