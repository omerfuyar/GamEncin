#include "Encin.h"
#include <stdio.h>

namespace GamEncin
{
    Application* Application::instance = nullptr;

    Application::Application()
    {
        if(instance)
            Application::instance->Stop(AppDuplicationErr);
        else
            instance = this;

        renderer = new Renderer();
    }

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
        currentScene->Awake();
        renderer->InitialRender(currentScene->objects);
    }

    void Application::Start()
    {
        currentScene->Start();
        renderer->RenderFrame(currentScene->objects);
    }

    void Application::Update()
    {
        currentScene->Update();
        renderer->RenderFrame(currentScene->objects);
    }

    void Application::FixUpdate()
    {
        currentScene->FixUpdate();
    }

    void Application::Run()
    {
        GameLoops();
    }

    void Application::GameLoops()
    {
        Awake();

        Start();

        fixedDeltaTime = 1.0 / (float) fixedFPS;
        steady_clock::time_point lastUpdate = high_resolution_clock::now();
        float fpsTimer = 0.0;

        while(!renderer->windowCloseInput)
        {
            steady_clock::time_point now = high_resolution_clock::now();
            deltaTime = duration<float>(now - lastUpdate).count();
            lastUpdate = now;

            accumulatedTime += deltaTime;
            secondsPastFromStart += deltaTime;

            Update();
            FPS++;

            while(accumulatedTime >= fixedDeltaTime)
            {
                FixUpdate();
                accumulatedTime -= fixedDeltaTime;
                //in that way, fixed update will be able to run multiple times in a frame if the frame rate is too low
                fpsTimer += fixedDeltaTime;
            }

            if(fpsTimer >= 1.0f)
            {
                if(printFPS)
                    printf("FPS: %d\n", FPS);
                fpsTimer = 0;
                FPS = 0;
            }
        }

        Stop(Safe);
    }

    void Application::Stop(EndType et)
    {
        switch(et)
        {
            case Safe:
                fprintf(stderr, "Program ended safely");
                break;
            case Warning:
                fprintf(stderr, "Program ended with warning(s)");
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
            case AppDuplicationErr:
                fprintf(stderr, "ERROR: Application duplicated");
                break;
            default:
                fprintf(stderr, "ERROR: Unknown error occurred");
                break;
        }

        printf(" :   %d\n", et);
        renderer->EndRenderer();
        exit(et);
    }
}
