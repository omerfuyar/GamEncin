#include "Encin/Encin.h"

namespace GamEncin
{
#pragma region Variable Definitions

    double Application::deltaTime = 0.0f;
    double Application::fixedDeltaTime = 0.0f;
    double Application::accumulatedTime = 0.0f;
    double Application::secondsPastFromStart = 0.0f;

    bool Application::printFPS = false;
    bool Application::isRunning = false;

    unsigned int Application::fixedFPS = 0;

    string Application::programName = "GamEncin";

    vector<Scene*> Application::scenes;

    Scene* Application::currentScene = nullptr;

#pragma endregion

    void Application::SetPrintFPS(bool value)
    {
        printFPS = value;
    }

    void Application::SetFixedFPS(unsigned int fps)
    {
        fixedFPS = Clamp(fps, 1, 1000);
        fixedDeltaTime = 1.0f / (float) fixedFPS;
    }

    void Application::SetProgramName(string name)
    {
        programName = name;
    }

    void Application::SetCurrentScene(Scene* scene)
    {
        if(!scene)
        {
            PrintLog(NullPointerErr, "Scene trying to set current is null");
            return;
        }

        auto obj = std::find(scenes.begin(), scenes.end(), scene);

        if(obj == scenes.end())
        {
            PrintLog(ElementDuplicationErr, "Scene trying to set current does not exist in application scenes");
            return;
        }

        currentScene = scene;
    }

    float Application::GetFixedDeltaTime()
    {
        return fixedDeltaTime;
    }

    float Application::GetDeltaTime()
    {
        return deltaTime;
    }

    float Application::GetAccumulatedTime()
    {
        return accumulatedTime;
    }

    float Application::GetSecondsPastFromStart()
    {
        return secondsPastFromStart;
    }

    bool Application::IsRunning()
    {
        return isRunning;
    }

    string Application::GetProgramName()
    {
        return programName;
    }

    Scene* const Application::GetCurrentScene()
    {
        return currentScene;
    }

    Scene& Application::CreateScene()
    {
        Scene* scene = new Scene();
        AddScene(scene);
        return *scene;
    }

    Scene& Application::CreateAndUseScene()
    {
        Scene* scene = &CreateScene();
        SetCurrentScene(scene);
        return *scene;
    }

    void Application::AddScene(Scene* scene)
    {
        if(!scene)
        {
            PrintLog(NullPointerErr, "Scene trying to add is null");
            return;
        }

        auto obj = std::find(scenes.begin(), scenes.end(), scene);

        if(obj != scenes.end())
        {
            PrintLog(ElementDuplicationErr, "Scene trying to add already exist");
            return;
        }

        scenes.push_back(scene);
    }

    void Application::Run()
    {
        if(!isRunning)
        {
            isRunning = true;
            GameLoops();
        }
        else
        {
            PrintLog(ProgramDuplicationErr, "Application is already working");
        }
    }

    void Application::PrintLog(LogType logType, string addMessage)
    {
        printf("\n");

        switch(logType)
        {
            case Safe:
                fprintf(stdout, "INFO: log is safe");
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
            case ElementCouldNotFindErr:
                fprintf(stderr, "ERROR: Element could not find");
                break;
            case TypeMismatchErr:
                fprintf(stderr, "ERROR: Type mismatch occurred");
                break;
            case IOErr:
                fprintf(stderr, "ERROR: Error occured while Input / Output actions");
                break;
            case ProgramDuplicationErr:
                fprintf(stderr, "ERROR: Program is already running");
                break;
            case NullPointerErr:
                fprintf(stderr, "ERROR: The pointer trying to access is null");
                break;
            case ElementDuplicationErr:
                fprintf(stderr, "ERROR: Element duplicated / already exist");
                break;
            case IndexOutOfRangeErr:
                fprintf(stderr, "ERROR: Index out of range");
                break;
            case idoiterr:
                fprintf(stderr, "ERROR: IDOITERR, idiot");
                break;
            case ComponentRequirementErr:
                fprintf(stderr, "ERROR: Component requirement error");
                break;
            default:
                fprintf(stdout, "UNKNOWN MESSAGE");
                break;
        }

        printf("\n");

        if(addMessage != "")
        {
            printf("Additional Message : %s\n", addMessage.c_str());
        }
    }

    void Application::Stop(LogType logType, string addMessage)
    {
        isRunning = false;

        printf("\nExit Code : %d\n", logType);

        PrintLog(logType, addMessage);

        if(isRunning)
        {
            Renderer::EndRenderer();
        }

        exit(logType);
    }

    void Application::GameLoops()
    {
        Awake();

        Start();

        fixedDeltaTime = 1.0f / (double) fixedFPS;
        steady_clock::time_point lastFrame = high_resolution_clock::now();
        double fpsTimer = 0.0f;

        while(!Renderer::GetWindowCloseInput())
        {
            steady_clock::time_point now = high_resolution_clock::now();
            deltaTime = duration<float>(now - lastFrame).count();
            lastFrame = now;

            accumulatedTime += deltaTime;
            secondsPastFromStart += deltaTime;
            fpsTimer += deltaTime;

            Update();

            while(accumulatedTime >= fixedDeltaTime)
            {
                FixUpdate();
                accumulatedTime -= fixedDeltaTime;
            }

            LateUpdate();

            if(fpsTimer >= 1.0f)
            {
                StartOfSecond();
                fpsTimer = 0;
            }
        }

        Stop(Safe, "End of game loops");
    }

    void Application::Awake()
    {
        srand(time(NULL));
        Renderer::InitialRender();
        TextureManager::InitializeTextures();
        Input::Initialize(Renderer::GetMainWindow());
        currentScene->Awake();
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
        Renderer::RenderFrame();
    }

    void Application::FixUpdate()
    {
        currentScene->FixUpdate();
        PhysicsManager::UpdateRigidBodies();
        PhysicsManager::ResolveCollisions();
    }

    void Application::StartOfSecond()
    {
        currentScene->StartOfSecond();

        if(printFPS)
        {
            char buff[100];
            snprintf(buff, sizeof(buff), "%s | FPS: %d | Delta Time: %f", programName.c_str(), (int) (1 / deltaTime), deltaTime);
            glfwSetWindowTitle(Renderer::GetMainWindow(), buff);
            printf("FPS: %d | Delta Time: %f\n", (int) (1 / deltaTime), deltaTime);
        }
    }
}
