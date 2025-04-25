#include "GamEncin.h"

#define GE_SELECT_OPTIMUM_GPU

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();

    Object& cameraObj = scene.CreateAndUseCameraObject();
    Camera* camera = cameraObj.GetComponent<Camera>();
    camera->SetCameraFOV(50.0f);
    camera->SetPerspective(true);
    camera->SetOrthographicSize(5.0f);
    camera->SetClipPlanes(0.1f, 300.0f);

    Transform* cameraTR = cameraObj.GetTransform();
    cameraTR->AddRotation(Vector3(0, -90, 0));
    cameraTR->AddPosition(Vector3(0, 0, 5));
    cameraTR = nullptr;

    CameraController* camController = cameraObj.AddComponent<CameraController>();

    Object& myPlane = scene.CreateObject();
    myPlane.SetName("plane");

    Transform* myObjectTR = myPlane.GetTransform();
    myObjectTR->AddPosition(Vector3(0, -5, 0));
    myObjectTR->AddScale(Vector3(3, 3, 3));

    Mesh* mesh = myPlane.AddComponent<Mesh>();
    mesh->SetMeshData(MeshBuilder::CreatePlane());
    mesh->SetMeshTexture(TextureManager::GetTexture("GamEncin/Resources/test3.jpg"));
    Renderer::AddMesh(mesh);

    RigidBody* rb1 = myPlane.AddComponent<RigidBody>();
    RigidBodyManager::AddRigidBody(rb1);
    rb1->SetDynamic(true);
    rb1->AddVelocity(Vector3(2, 0, 0));
    camController->planeRB = rb1;
    rb1->SetColliderRadius(3.0f);
    rb1->SetGravityScale(-1.0f);

    Object& mySphere = scene.CreateObject();
    mySphere.SetName("sphere");

    Transform* myObjectTR1 = mySphere.GetTransform();
    myObjectTR1->AddPosition(Vector3(0, 5, 0));

    Mesh* mesh2 = mySphere.AddComponent<Mesh>();
    mesh2->SetMeshData(MeshBuilder::CreateSphere());
    mesh2->SetMeshTexture(TextureManager::GetTexture("GamEncin/Resources/test.jpg"));
    Renderer::AddMesh(mesh2);

    RigidBody* rb2 = mySphere.AddComponent<RigidBody>();
    RigidBodyManager::AddRigidBody(rb2);
    rb2->SetDynamic(true);
    rb2->AddVelocity(Vector3(2, 0, 0));
    camController->sphereRB = rb2;

    int totalVerticeCount = 0;
    int totalIndiceCount = 0;
    int totalObjectCount = 0;

    // int side = 10;
    // float gap = 0.01f;
    // for(int i = 0; i < side; i++)
    //{
    //     for(int j = 0; j < side; j++)
    //     {
    //         Object& myPlane = scene.CreateObject();
    //         myPlane.tag = "myObj";
    //         Transform* myObjectTR = myPlane.GetTransform();
    //         Mesh* mesh2 = myPlane.AddComponent<Mesh>();
    //         mesh2->SetMeshData(MeshBuilder::CreateCube());
    //         mesh2->SetMeshTexture(TextureManager::GetTexture(RandomRangeInteger(0, 1) == 0 ? "GamEncin/src/Resources//test3.jpg" : /"GamEncin/src/Resources/test.jpg"));
    //         Renderer::AddMesh(mesh2);
    //
    //         MyComponent* myComponent = myPlane.AddComponent<MyComponent>();
    //
    //         totalIndiceCount += mesh2->meshData.faces.size() * 3;
    //         totalVerticeCount += mesh2->meshData.vertices.size();
    //         totalObjectCount++;
    //
    //         myObjectTR->AddPosition(Vector3(j - side / 2, i - side / 2, -1));
    //         myObjectTR->AddRotation(Vector3(j, i, 0));
    //     }
    // }

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
