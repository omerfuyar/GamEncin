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
    cameraTR = nullptr;

    CameraController* camController = cameraObj.AddComponent<CameraController>();
    camController->LockMovement(true);

    ////////////////

    Texture* fontTex = TextureManager::GetTexture("GamEncin/Resources/Fonts/myFont.png");
    Font* myFont = FontManager::GetFont("GamEncin/Resources/Fonts/Garamond/garamond.bdf");

    /////////////////

    Object& sphere1 = scene.CreateObject();
    Mesh* sphereMesh1 = sphere1.AddComponent<Mesh>();
    sphereMesh1->SetMeshData(MeshBuilder::CreateSphere(1.0f));
    sphereMesh1->SetMeshTexture(TextureManager::GetTexture("GamEncin/Resources/Textures/test.jpg"));
    Renderer::AddMesh(sphereMesh1);

    RigidBody* sphereRB1 = sphere1.AddComponent<RigidBody>();
    PhysicsManager::AddRigidBody(sphereRB1);
    sphereRB1->SetDynamic(false);
    sphereRB1->SetGravityScale(0);

    //Text* text1 = sphere1.AddComponent<Text>();
    //text1->SetFont(myFont);
    //text1->SetText("Static");
    //Renderer::AddText(text1);

    /////////////////

    Object& sphere2 = scene.CreateObject();
    Mesh* sphereMesh2 = sphere2.AddComponent<Mesh>();
    sphereMesh2->SetMeshData(MeshBuilder::CreateSphere(1.0f));
    sphereMesh2->SetMeshTexture(TextureManager::GetTexture("GamEncin/Resources/Textures/test3.jpg"));
    Renderer::AddMesh(sphereMesh2);

    RigidBody* sphereRB2 = sphere2.AddComponent<RigidBody>();
    PhysicsManager::AddRigidBody(sphereRB2);
    sphereRB2->SetDynamic(true);
    sphereRB2->SetGravityScale(0);

    Transform* sphereTR2 = sphere2.GetTransform();
    sphereTR2->AddPosition(Vector3(0, 5, 0));

    //Text* text2 = sphere2.AddComponent<Text>();
    //text2->SetFont(myFont);
    //text2->SetText("Static");
    //Renderer::AddText(text2);

    sphere2.AddComponent<PlayerController>();

    return;
    ////////////////

    Object& mySphere1 = scene.CreateObject();
    mySphere1.SetName("sphere");

    Transform* myObjectTR1 = mySphere1.GetTransform();
    myObjectTR1->AddPosition(Vector3(0, 5, 0));

    Mesh* mesh1 = mySphere1.AddComponent<Mesh>();
    mesh1->SetMeshData(MeshBuilder::CreateSphere(1.0f));
    mesh1->SetMeshTexture(TextureManager::GetTexture("GamEncin/Resources/test.jpg"));
    Renderer::AddMesh(mesh1);

    RigidBody* rb1 = mySphere1.AddComponent<RigidBody>();
    PhysicsManager::AddRigidBody(rb1);
    rb1->SetDynamic(true);
    rb1->SetGravityScale(0);
    rb1->AddVelocity(Vector3(0, RandomRangeFloat(-15, 0), 0));

    //////////////////////

    Object& mySphere2 = scene.CreateObject();
    mySphere2.SetName("sphere2");

    Transform* myObjectTR2 = mySphere2.GetTransform();
    myObjectTR2->AddPosition(Vector3(0, -5, 0));

    Mesh* mesh2 = mySphere2.AddComponent<Mesh>();
    mesh2->SetMeshData(MeshBuilder::CreateSphere(3.0f));
    mesh2->SetMeshTexture(TextureManager::GetTexture("GamEncin/Resources/test.jpg"));
    Renderer::AddMesh(mesh2);

    RigidBody* rb2 = mySphere2.AddComponent<RigidBody>();
    PhysicsManager::AddRigidBody(rb2);
    rb2->SetDynamic(true);
    rb2->SetColliderRadius(3.0f);
    rb2->SetGravityScale(0);
    rb2->SetMass(3.0f);
    rb2->AddVelocity(Vector3(0, RandomRangeFloat(0, 15), 0));

    ////////////////////////

    Object& mySphere3 = scene.CreateObject();
    mySphere3.SetName("sphere3");

    Transform* TR3 = mySphere3.GetTransform();
    TR3->AddPosition(Vector3(0, 10, 0));

    Mesh* mesh3 = mySphere3.AddComponent<Mesh>();
    mesh3->SetMeshData(MeshBuilder::CreateSphere(5.0f));
    mesh3->SetMeshTexture(TextureManager::GetTexture("GamEncin/Resources/test3.jpg"));
    Renderer::AddMesh(mesh3);

    RigidBody* rb3 = mySphere3.AddComponent<RigidBody>();
    PhysicsManager::AddRigidBody(rb3);
    rb3->SetDynamic(false);
    rb3->SetColliderRadius(5.0f);

    ////////////////////////

    Object& mySphere4 = scene.CreateObject();
    mySphere4.SetName("sphere4");

    Transform* TR4 = mySphere4.GetTransform();
    TR4->AddPosition(Vector3(0, -20, 0));

    Mesh* mesh4 = mySphere4.AddComponent<Mesh>();
    mesh4->SetMeshData(MeshBuilder::CreateSphere(5.0f));
    mesh4->SetMeshTexture(TextureManager::GetTexture("GamEncin/Resources/test3.jpg"));
    Renderer::AddMesh(mesh4);

    RigidBody* rb4 = mySphere4.AddComponent<RigidBody>();
    PhysicsManager::AddRigidBody(rb4);
    rb4->SetDynamic(false);
    rb4->SetColliderRadius(5.0f);
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
