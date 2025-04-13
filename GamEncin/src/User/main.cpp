#include "GamEncin.h"

//#define GE_SELECT_OPTIMUM_GPU

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();
    Object& cameraObj = scene.CreateAndUseCameraObject();
    cameraObj.AddComponent<CameraController>();
    Camera* camera = cameraObj.GetComponent<Camera>();
    camera->SetCameraFOV(50.0f);
    camera->SetPerspective(true);
    camera->SetClipPlanes(0.1f, 300.0f);

    Object& myObject = scene.CreateObject();

    Mesh* mesh = myObject.AddComponent<Mesh>();
    mesh->SetMeshData(MeshBuilder::CreateCube());

    MyComponent* myComponent = myObject.AddComponent<MyComponent>();

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
    Application::SetFixedFPS(50);
    Application::SetFPSPrint(true);
    Application::SetProgramName("GamEncin");
    Renderer::SetWindowProperties(false, false, Vector2Int(1080, 720), Vector4(0.2f, 0.3f, 0.3f, 1.0f));
}

int main()
{
    SceneBuilding();
    SetVariables();

    Application::Run();

    return 0;
}

#ifdef GE_SELECT_OPTIMUM_GPU
#include <Windows.h>
extern "C"
{
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    _declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif
