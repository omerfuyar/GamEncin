#include "GamEncin.h"

#define GE_SELECT_OPTIMUM_GPU

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();

    Object& cameraObj = scene.CreateAndUseCameraObject();

    Transform* cameraTR = cameraObj.transform;
    cameraTR->AddRotation(Vector3(0, -90, 0));

    CameraController* camController = cameraObj.AddComponent<CameraController>();

    Camera* camera = cameraObj.GetComponent<Camera>();
    camera->SetCameraFOV(50.0f);
    camera->SetPerspective(true);
    camera->SetClipPlanes(0.1f, 300.0f);

    int totalVerticeCount = 0;
    int totalIndiceCount = 0;
    int totalObjectCount = 0;

    int side = 101;
    float gap = 0.001f;
    for(int i = 0; i < side; i++)
    {
        for(int j = 0; j < side; j++)
        {
            Object& myObject = scene.CreateObject();
            Transform* myObjectTR = myObject.transform;
            Mesh* mesh = myObject.AddComponent<Mesh>();
            mesh->SetMeshData(MeshBuilder::CreateCube());

            MyComponent* myComponent = myObject.AddComponent<MyComponent>();

            totalIndiceCount += mesh->meshData.faces.size() * 3;
            totalVerticeCount += mesh->meshData.vertices.size();
            totalObjectCount++;
            myObjectTR->AddPosition(Vector3((i - side / 2.0f), (j - side / 2.0f), 0) * gap);
            myObjectTR->AddPosition(Vector3(0, 0, -10));
            myObjectTR->SetLocalRotation(Vector3((i + j), (i + j), (i + j)) * gap * 10);
        }
    }

    printf("total vertice count: %d\n", totalVerticeCount);
    printf("total indice count: %d\n", totalIndiceCount);
    printf("total triangle count: %d\n", totalIndiceCount / 3);
    printf("total object count: %d\n", totalObjectCount);
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
    Input::GetExeFilePath();
    SceneBuilding();
    SetVariables();

    Application::Run();

    return 0;
}

#ifdef GE_SELECT_OPTIMUM_GPU
//#include <windows.h>
extern "C"
{
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    _declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif
