#include "GamEncin.h"

#define GE_SELECT_OPTIMUM_GPU

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();

    ///////////////// camera

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
    camController->LockMovement(true);

    //////////////// resources

    Font* myFont = FontManager::GetFont("GamEncin/Resources/Fonts/Garamond/garamond.bdf");
    Texture* texture1 = TextureManager::GetTexture("GamEncin/Resources/Textures/test.jpg");
    Texture* texture2 = TextureManager::GetTexture("GamEncin/Resources/Textures/test3.jpg");

    ///////////////// static sphere

    Object& sphere1 = scene.CreateObject();
    sphere1.SetName("Static Sphere");
    ModelMesh* sphereMesh1 = sphere1.AddComponent<ModelMesh>();
    sphereMesh1->SetMeshData(MeshBuilder::CreateSphere(1.0f));
    sphereMesh1->SetTexture(texture1);

    RigidBody* sphereRB1 = sphere1.AddComponent<RigidBody>();
    sphereRB1->SetDynamic(false);
    sphereRB1->SetGravityScale(0);

    ///////////////// player sphere

    Object& sphere2 = scene.CreateObject();
    sphere2.SetName("Player Sphere");
    ModelMesh* sphereMesh2 = sphere2.AddComponent<ModelMesh>();
    sphereMesh2->SetMeshData(MeshBuilder::CreateSphere(1.0f));
    sphereMesh2->SetTexture(texture2);

    RigidBody* sphereRB2 = sphere2.AddComponent<RigidBody>();
    sphereRB2->SetDynamic(true);
    sphereRB2->SetGravityScale(0);

    Transform* sphereTR2 = sphere2.GetTransform();
    sphereTR2->AddPosition(Vector3(2.5f, 0, 0));

    PlayerController* playerController = sphere2.AddComponent<PlayerController>();

    ///////////////// dynamic test sphere

    Object& sphere3 = scene.CreateObject();
    sphere3.SetName("Dynamic Sphere");
    ModelMesh* sphereMesh3 = sphere3.AddComponent<ModelMesh>();
    sphereMesh3->SetMeshData(MeshBuilder::CreateSphere(1.0f));
    sphereMesh3->SetTexture(texture1);

    RigidBody* sphereRB3 = sphere3.AddComponent<RigidBody>();
    sphereRB3->SetDynamic(true);
    sphereRB3->SetGravityScale(1.0f);

    Transform* sphereTR3 = sphere3.GetTransform();
    sphereTR3->AddPosition(Vector3(0, 2.5f, 0));

    ///////////////// text

    //Object& textObj1 = sphere2.CreateChildObject();
    //textObj1.GetTransform()->AddPosition(Vector3(0, 0, 2));
    //TextMesh* text1 = textObj1.AddComponent<TextMesh>();
    //text1->SetFont(myFont);
    //text1->SetCharDistance(0.01f);
    //text1->SetTextSize(Vector2(3, 3));
    //text1->SetText("Dynamic");

    Renderer::AddMeshesInScene(&scene);
    PhysicsManager::AddRigidBodiesInScene(&scene);
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
