#include "GamEncin.h"

#define GE_SELECT_OPTIMUM_GPU

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();

    Object& cameraObj = scene.CreateAndUseCameraObject();
    Camera* camera = cameraObj.GetComponent<Camera>();
    camera->SetCameraFOV(50.0f);
    camera->SetPerspective(true);
    camera->SetClipPlanes(0.1f, 300.0f);

    Transform* cameraTR = cameraObj.transform;
    cameraTR->AddRotation(Vector3(0, -90, 0));

    CameraController* camController = cameraObj.AddComponent<CameraController>();

    int totalVerticeCount = 0;
    int totalIndiceCount = 0;
    int totalObjectCount = 0;

    Object& myObject2 = scene.CreateObject();
    Mesh* mesh1 = myObject2.AddComponent<Mesh>();
    myObject2.AddComponent<MyComponent>();
    mesh1->SetMeshData(MeshBuilder::CreateCube());
    mesh1->SetMeshTexture(TextureManager::GetTexture("GamEncin/src/Resources/test.jpg"));
    myObject2.transform->AddPosition(Vector3(0, 0, -1.0f));
    Renderer::AddMesh(mesh1);


    totalIndiceCount += mesh1->meshData.faces.size() * 3;
    totalVerticeCount += mesh1->meshData.vertices.size();
    totalObjectCount++;

    //int side = 101;
    //float gap = 0.01f;
    //for(int i = 0; i < side; i++)
    //{
    //    for(int j = 0; j < side; j++)
    //    {
    //        Object& myObject = scene.CreateObject();
    //        myObject.tag = "myObj";
    //        Transform* myObjectTR = myObject.transform;
    //        Mesh* mesh = myObject.AddComponent<Mesh>();
    //        mesh->SetMeshData(MeshBuilder::CreateCube());
    //        Renderer::AddMesh(mesh);
    //        myObject.transform->SetParent(myObject2.transform);
    //
    //        MyComponent* myComponent = myObject.AddComponent<MyComponent>();
    //
    //        totalIndiceCount += mesh->meshData.faces.size() * 3;
    //        totalVerticeCount += mesh->meshData.vertices.size();
    //        totalObjectCount++;
    //
    //        myObjectTR->AddPosition(Vector3(j - side / 2, i - side / 2, -1));
    //        myObjectTR->AddRotation(Vector3(j, i, 0));
    //    }
    //}

    printf("\ntotal vertice count: %d\n", totalVerticeCount);
    printf("total indice count: %d\n", totalIndiceCount);
    printf("total triangle count: %d\n", totalIndiceCount / 3);
    printf("total object count: %d\n\n", totalObjectCount);
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
extern "C"
{
    _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    _declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif
