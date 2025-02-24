#include "Encin.h"
#include <stdio.h>

namespace GamEncin
{
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
        renderer->InitialRender();
    }

    void Application::Start()
    {
        currentScene->Start();
        renderer->RenderFrame();
    }

    void Application::Update()
    {
        currentScene->Update();
        renderer->RenderFrame();
    }

    void Application::FixUpdate()
    {
        currentScene->FixUpdate();
    }

    void Application::Run()
    {
        renderer = new Renderer;
        GameLoops();
    }

    void Application::GameLoops()
    {
        Awake();

        Start();

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
                fprintf(stderr, "Program ended safely\n" + et);
                break;
            case Warning:
                fprintf(stderr, "Program ended with warning(s)\n");
                break;
            case GLFWErr:
                fprintf(stderr, "ERROR: Error occurred in GLFW3\n");
                break;
            case GLADErr:
                fprintf(stderr, "ERROR: Error occurred in GLAD\n");
                break;
            case ObjCouldNotFoundErr:
                fprintf(stderr, "ERROR: Object could not be found\n");
                break;
            case TypeMismachErr:
                fprintf(stderr, "ERROR: Type mismatch occurred\n");
                break;
            case IOErr:
                fprintf(stderr, "ERROR: Error occured while Input / Output actions\n");
                break;
            default:
                fprintf(stderr, "ERROR: Unknown error occurred\n");
                break;
        }

        renderer->EndRenderer();
        exit(et);
    }
}
