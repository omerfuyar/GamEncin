#include "Encin/Encin.h"

namespace GamEncin
{
#pragma region Variable Definitions

    vector<Scene*> Application::scenes;
    Scene* Application::currentScene = nullptr;
    GLFWwindow* Application::window = nullptr;
    string Application::programName = "GamEncin Default";

    int Application::fixedFPS = 0;

    float Application::deltaTime = 0.0f,
        Application::fixedDeltaTime = 0.0f,
        Application::accumulatedTime = 0.0f,
        Application::secondsPastFromStart = 0.0f;

    bool Application::printFPS = false,
        Application::isRunning = false;

#pragma endregion

    Scene& Application::CreateScene()
    {
        Scene* scene = new Scene();
        scenes.push_back(scene);
        return *scene;
    }

    void Application::SetCurrentScene(Scene* scene)
    {
        currentScene = scene;
    }

    void Application::SetFixedFPS(int fps)
    {
        fixedFPS = fps;
    }

    void Application::SetProgramName(string name)
    {
        programName = name;
    }

    void Application::SetFPSPrint(bool value)
    {
        printFPS = value;
    }

    Scene* Application::GetCurrentScene()
    {
        return currentScene;
    }

    GLFWwindow* Application::GetMainWindow()
    {
        return window;
    }

    string Application::GetProgramName()
    {
        return programName;
    }

    Scene& Application::CreateAndUseScene()
    {
        Scene* scene = new Scene();
        scenes.push_back(scene);
        SetCurrentScene(scene);
        return *scene;
    }

    void Application::Awake()
    {
        srand(time(NULL));
        Renderer::InitialRender();//TODO 
        window = Renderer::GetMainWindow();
        currentScene->Awake();
        Input::Initialize(window); //after the window is created
    }

    void Application::Start()
    {
        currentScene->Start();
        Renderer::RenderFrame();
    }

    void Application::Update()
    {
        Input::UpdateInputs();
        currentScene->Update();
    }

    void Application::LateUpdate()
    {
        currentScene->LateUpdate();
        Renderer::RenderFrame();
    }

    void Application::FixUpdate()
    {
        currentScene->FixUpdate();
    }

    void Application::StartOfSecond()
    {
        currentScene->StartOfSecond();

        if(printFPS)
        {
            char buff[100];
            snprintf(buff, sizeof(buff), "%s | FPS: %d | Delta Time: %f", programName.c_str(), (int) (1 / deltaTime), deltaTime);
            glfwSetWindowTitle(window, buff);
            printf("FPS: %d | Delta Time: %f\n", (int) (1 / deltaTime), deltaTime);
        }
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

        while(!Renderer::GetWindowCloseInput())
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

            if(fpsTimer >= 1.0f)
            {
                StartOfSecond();
                fpsTimer = 0;
            }
        }

        Stop(Safe);
    }

    void Application::PrintLog(LogType logType)
    {
        printf("\n");

        switch(logType)
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
            case ElementCouldNotFoundErr:
                fprintf(stderr, "ERROR: Element could not be found");
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
            case NullPointerErr:
                fprintf(stderr, "ERROR: Null pointer exception");
                break;
            default:
                fprintf(stdout, "UNKNOWN MESSAGE");
                break;
        }

        printf("\n");
    }

    void Application::PrintLog(LogType logType, string addMessage)
    {
        PrintLog(logType);
        printf("Additional Message : %s\n\n", addMessage.c_str());
    }

    void Application::Stop(LogType logType)
    {
        isRunning = false;
        printf("\nExit Code : %d\n", logType);
        PrintLog(logType);
        Renderer::EndRenderer();
        for(Scene* scene : scenes)
            delete scene;
        scenes.clear();
        exit(logType);
    }

    void Application::Stop(LogType logType, string addMessage)
    {
        isRunning = false;
        printf("\nExit Code : %d\n", logType);
        PrintLog(logType, addMessage);
        Renderer::EndRenderer();
        for(Scene* scene : scenes)
            delete scene;
        scenes.clear();
        exit(logType);
    }
}
