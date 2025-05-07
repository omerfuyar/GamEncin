#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Scene;

    class Application //singleton
    {
    public:
        //Sets whether to print the FPS in the console and in title or not
        static void SetPrintFPS(bool value);
        //Changes the FixUpdate FPS
        static void SetFixedFPS(unsigned int fps);
        //Changes the name of the application and also window title
        static void SetProgramName(string name);
        //Sets the application's currently active scene
        static void LoadScene(Scene* scene);
        //Loads the next scene in the scenes vector
        static void LoadNextScene();

        //Returns the interval between fixed updates in seconds
        static float GetFixedDeltaTime();
        //Returns the duration of the last frame in seconds
        static float GetDeltaTime();
        //Returns the time has passed since the last fixed update in seconds
        static float GetAccumulatedTime();
        //Returns the time has passed since the start of the program in seconds
        static float GetSecondsPastFromStart();
        //Returns whether the program is running or not
        static bool IsRunning();
        //Returns the name of the application
        static string GetProgramName();
        //Returns the application's currently active scene
        static Scene* const GetCurrentScene();

        static void AddScene(Scene* scene);
        static void Run();
        static void PrintLog(LogType endType, string addMessage = "");
        static void Stop(LogType endType, string addMessage = "");

    private:
        static double fixedDeltaTime;
        static double deltaTime;
        static double accumulatedTime;
        static double secondsPastFromStart;

        static bool printFPS;
        static bool isRunning;

        static unsigned int fixedFPS;

        static string programName;

        static vector<Scene*> scenes;

        static Scene* currentScene;

        Application() = delete;
        Application(const Application&) = delete;
        void operator=(const Application&) = delete;

        static void GameLoops();
        static void Awake();
        static void Start();
        static void Update();
        static void LateUpdate();
        static void FixUpdate();
        static void StartOfSecond();
    };
}
