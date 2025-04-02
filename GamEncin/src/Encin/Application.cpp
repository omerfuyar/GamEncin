#include "Encin.h"
#include <stdio.h>
#include <time.h>

namespace GamEncin
{

#pragma region Variable Definitions

    Scene* Application::currentScene = nullptr;
    string Application::programName = "GamEncin Default";
    vector<Scene*> Application::scenes;

    int Application::FPS = 0,
        Application::frameCount = 0,
        Application::fixedFPS = 0;

    float Application::deltaTime = 0.0,
        Application::fixedDeltaTime = 0.0,
        Application::accumulatedTime = 0.0,
        Application::secondsPastFromStart = 0.0;

    bool Application::printFPS = false,
        Application::isRunning = false;

#pragma endregion

    Scene& Application::CreateScene()
    {
        Scene* scene = new Scene();
        scenes.push_back(scene);
        return *scene;
    }

    void Application::SetCurrentScene(Scene& scene)
    {
        currentScene = &scene;
    }

    Scene& Application::CreateAndUseScene()
    {
        Scene* scene = new Scene();
        scenes.push_back(scene);
        SetCurrentScene(*scene);
        return *scene;
    }

    void Application::Awake()
    {
        srand(time(NULL));
        currentScene->Awake();
        Input::Initialize(currentScene->renderer->window); //after the window is created
    }

    void Application::Start()
    {
        currentScene->Start();
    }

    void Application::Update()
    {
        Input::UpdateInputs();
        currentScene->Update();
    }

    void Application::LateUpdate()
    {
        currentScene->LateUpdate();
    }

    void Application::FixUpdate()
    {
        currentScene->FixUpdate();
    }

    void Application::Run()
    {
        if(!isRunning)
        {
            GameLoops();
            isRunning = true;
        }
        else
        {
            Stop(ProgramDuplicationErr);
        }
    }

    void Application::GameLoops()
    {
        Awake();

        Start();

        fixedDeltaTime = 1.0 / (float) fixedFPS;
        steady_clock::time_point lastFrame = high_resolution_clock::now();
        float fpsTimer = 0.0;

        while(!currentScene->renderer->windowCloseInput)
        {
            steady_clock::time_point now = high_resolution_clock::now();
            deltaTime = duration<float>(now - lastFrame).count();
            lastFrame = now;

            accumulatedTime += deltaTime;
            secondsPastFromStart += deltaTime;
            fpsTimer += deltaTime;

            while(accumulatedTime >= fixedDeltaTime)
            {
                FixUpdate();
                accumulatedTime -= fixedDeltaTime;
            }

            Update();
            LateUpdate();
            frameCount++;

            if(fpsTimer >= 1.0f)
            {
                FPS = frameCount;

                if(printFPS)
                {
                    char buff[100];
                    snprintf(buff, sizeof(buff), "%s | FPS: %d", programName.c_str(), FPS);
                    glfwSetWindowTitle(currentScene->renderer->window, buff);
                }

                fpsTimer = 0;
                frameCount = 0;
            }
        }

        Stop(Safe);
    }

    void Application::PrintLog(EndType endType)
    {
        printf("\nExit Code : %d\n", endType);

        switch(endType)
        {
            case Safe:
                fprintf(stdout, "INFO: Program ended safely");
                break;
            case IODeviceWarn:
                fprintf(stdout, "WARNING: Warning occurred in Input / Output device(s)");
                break;
            case GLFWErr:
                fprintf(stderr, "ERROR: Error occurred in GLFW3");
                break;
            case GLADErr:
                fprintf(stderr, "ERROR: Error occurred in GLAD");
                break;
            case ShaderCompilationErr:
                fprintf(stderr, "ERROR: Error occurred while compiling shaders");
                break;
            case ShaderLinkingErr:
                fprintf(stderr, "ERROR: Error occurred while linking shaders");
                break;
            case ObjCouldNotFoundErr:
                fprintf(stderr, "ERROR: Object could not be found");
                break;
            case TypeMismachErr:
                fprintf(stderr, "ERROR: Type mismatch occurred");
                break;
            case IOErr:
                fprintf(stderr, "ERROR: Error occured while Input / Output actions");
                break;
            case ProgramDuplicationErr:
                fprintf(stderr, "ERROR: Program is already running");
                break;
            default:
                fprintf(stdout, "UNKNOWN MESSAGE");
                break;
        }

        printf("\n");
    }

    void Application::PrintLog(EndType endType, const char* addMessage)
    {
        PrintLog(endType);
        printf("Additional Message : %s\n", addMessage);
    }

    void Application::Stop(EndType endType)
    {
        isRunning = false;
        PrintLog(endType);
        currentScene->renderer->EndRenderer();
        for(Scene* scene : scenes)
            delete scene;
        scenes.clear();
        exit(endType);
    }

    void Application::Stop(EndType endType, const char* addMessage)
    {
        isRunning = false;
        PrintLog(endType, addMessage);
        currentScene->renderer->EndRenderer();
        for(Scene* scene : scenes)
            delete scene;
        scenes.clear();
        exit(endType);
    }
}
