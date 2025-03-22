#include "GamEncin.h"

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();

    myObject* obj = new myObject();
    scene.AddObject(obj);
}

void SetVariables()
{
    Application::fixedFPS = 50;
    Application::printFPS = true;
    Application::currentScene->renderer->initWindowSize = Vector2(1080, 1080);
    Application::currentScene->renderer->clearColor = Vector4(0.2, 0.3, 0.3, 1.0);
    Application::currentScene->renderer->camera->cameraFOV = 50.0;
}

int main()
{
    SceneBuilding();
    SetVariables();

    Application::Run();

    return 0;
}
