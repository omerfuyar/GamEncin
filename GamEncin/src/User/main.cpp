#include "GamEncin.h"

#define GE_SELECT_OPTIMUM_GPU

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();

    Object& cameraObj = scene.CreateAndUseCameraObject();
    Camera* camera = cameraObj.GetComponent<Camera>();
    camera->SetCameraFOV(50.0f);
    camera->SetPerspective(false);
    camera->SetOrthographicSize(5.0f);
    camera->SetClipPlanes(0.1f, 300.0f);

    Transform* cameraTR = cameraObj.GetTransform();
    cameraTR->AddRotation(Vector3(0, -90, 0));
    cameraTR->AddPosition(Vector3(0, 0, 5));

    CameraController* camController = cameraObj.AddComponent<CameraController>();

    Object& myObject = scene.CreateObject();
    myObject.SetTag("myObj");
    Transform* myObjectTR = myObject.GetTransform();
    Mesh* mesh = myObject.AddComponent<Mesh>();
    mesh->SetMeshData(MeshBuilder::CreateCube());
    mesh->SetMeshTexture(TextureManager::GetTexture(RandomRangeInteger(0, 1) == 0 ? "GamEncin/src/Resources/test3.jpg" : "GamEncin/src/Resources/test.jpg"));
    Renderer::AddMesh(mesh);

    MyComponent* myComponent = myObject.AddComponent<MyComponent>();

    int totalVerticeCount = 0;
    int totalIndiceCount = 0;
    int totalObjectCount = 0;

    //int side = 10;
    //float gap = 0.01f;
    //for(int i = 0; i < side; i++)
    //{
    //    for(int j = 0; j < side; j++)
    //    {
    //        Object& myObject = scene.CreateObject();
    //        myObject.tag = "myObj";
    //        Transform* myObjectTR = myObject.GetTransform();
    //        Mesh* mesh = myObject.AddComponent<Mesh>();
    //        mesh->SetMeshData(MeshBuilder::CreateCube());
    //        mesh->SetMeshTexture(TextureManager::GetTexture(RandomRangeInteger(0, 1) == 0 ? "GamEncin/src/Resources//test3.jpg" : /"GamEncin/src/Resources/test.jpg"));
    //        Renderer::AddMesh(mesh);
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
    Application::SetPrintFPS(true);
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
