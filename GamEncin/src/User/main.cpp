#include "GamEncin.h"

GE_SELECT_OPTIMUM_GPU

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();
    Object* camera = scene.CreateAndUseCameraObject(Vector2Int(1080, 1080));
    camera->AddComponent<CameraController>();

    Object* myObject = scene.CreateObject();
    //Object* myObject = scene.CreateObject<Mesh>();
    Mesh* mesh = myObject->AddComponent<Mesh>();
    Transform* transform = myObject->GetComponent<Transform>();


    //int totalVerticeCount = 0;
    //int totalIndiceCount = 0;
    //int totalObjectCount = 0;
    //
    //int side = 101;
    //float gap = 0.01f;
    //for(int i = 0; i < side; i++)
    //{
    //    for(int j = 0; j < side; j++)
    //    {
    //        myObject* shape = scene.CreateObject<myObject>();
    //        totalIndiceCount += shape->indices.size();
    //        totalVerticeCount += shape->vertices.size();
    //        totalObjectCount++;
    //        shape->position = Vector3((i - side / 2.0f), (j - side / 2.0f), 0) * gap;
    //        shape->position -= Vector3(0, 0, 10);
    //        shape->rotation = Vector3((i + j), (i + j), (i + j)) * gap * 10;
    //    }
    //}

    //printf("Sphere vertice count: %d\n", totalVerticeCount);
    //printf("Sphere indice count: %d\n", totalIndiceCount);
    //printf("Sphere triangle count: %d\n", totalIndiceCount / 3);
    //printf("Sphere object count: %d\n", totalObjectCount);
}

void SetVariables()
{
    Application::fixedFPS = 50;
    Application::printFPS = true;
    Application::programName = "GamEncin";
    Application::currentScene->renderer->initWindowSize = Vector2Int(1080, 1080);
    Application::currentScene->renderer->clearColor = Vector4(0.2, 0.3, 0.3, 1.0);
    Application::currentScene->renderer->mainCamera->cameraFOV = 50.0;
}

int main()
{
    SceneBuilding();
    SetVariables();

    Application::Run();

    return 0;
}
