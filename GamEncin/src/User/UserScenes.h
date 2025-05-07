#pragma once
#include "GamEncin.h"

class TestScene : public Scene
{
public:
    TestScene(string name) : Scene(name)
    {
    }

    void BuildScene() override
    {
        //////////////// resources

        Font* myFont1 = FontManager::GetFont("GamEncin/Resources/Fonts/Garamond/garamond.bdf");
        Font* myFont2 = FontManager::GetFont("GamEncin/Resources/Fonts/Tamzen/Tamzen10x20b.bdf");
        Texture* texture1 = TextureManager::GetTexture("GamEncin/Resources/Textures/test.jpg");
        Texture* texture2 = TextureManager::GetTexture("GamEncin/Resources/Textures/test3.jpg");

        /////////////// camera

        Object& cameraObj = CreateAndUseCameraObject();
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

        ///////////////// static sphere

        Object& sphere1 = CreateObject();
        sphere1.SetName("Static Sphere");
        ModelMesh* sphereMesh1 = sphere1.AddComponent<ModelMesh>();
        sphereMesh1->SetMeshData(MeshBuilder::CreateSphere(1.0f));
        sphereMesh1->SetTexture(texture1);

        RigidBody* sphereRB1 = sphere1.AddComponent<RigidBody>();
        sphereRB1->SetDynamic(false);
        sphereRB1->SetGravityScale(0);

        sphere1.AddComponent<TestComponent>();

        ///////////////// player sphere

        Object& playerObj = CreateObject();
        playerObj.SetName("Player Sphere");
        ModelMesh* sphereMesh2 = playerObj.AddComponent<ModelMesh>();
        sphereMesh2->SetMeshData(MeshBuilder::CreateSphere(1.0f));
        sphereMesh2->SetTexture(texture2);

        RigidBody* playerRB = playerObj.AddComponent<RigidBody>();
        playerRB->SetDynamic(true);
        playerRB->SetGravityScale(0);

        Transform* playerTR = playerObj.GetTransform();
        playerTR->AddPosition(Vector3(2.5f, 0, 0));

        PlayerController* playerController = playerObj.AddComponent<PlayerController>();

        ///////////////// dynamic test sphere

        Object& sphere3 = CreateObject();
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

        Object& textObj1 = playerObj.CreateChildObject();
        textObj1.SetName("Player Text");
        textObj1.GetTransform()->SetLocalPosition(Vector3(0, 0, 2));
        TextMesh* text1 = textObj1.AddComponent<TextMesh>();
        text1->SetFont(myFont1);
        text1->SetCharDistance(1.5f);
        text1->SetTextSize(Vector2(5, 5));
        text1->SetText("Player");

        Object& textObj2 = sphere1.CreateChildObject();
        textObj2.SetName("Static Text");
        textObj2.GetTransform()->SetLocalPosition(Vector3(0, 0, 2));
        TextMesh* text2 = textObj2.AddComponent<TextMesh>();
        text2->SetFont(myFont2);
        text2->SetCharDistance(1.5f);
        text2->SetTextSize(Vector2(5, 5));
        text2->SetText("Ne Diyo\nLaa");

        Object& textObj3 = sphere3.CreateChildObject();
        textObj3.SetName("Dynamic Text");
        textObj3.GetTransform()->SetLocalPosition(Vector3(0, 0, 2));
        TextMesh* text3 = textObj3.AddComponent<TextMesh>();
        text3->SetFont(myFont2);
        text3->SetCharDistance(1.5f);
        text3->SetTextSize(Vector2(5, 5));
        text3->SetText("OOO 0000 OOOluum");
    }
};

class TestScene2 : public Scene
{
public:
    TestScene2(string name) : Scene(name)
    {
    }

    void BuildScene() override
    {
        //////////////// resources

        Font* myFont1 = FontManager::GetFont("GamEncin/Resources/Fonts/Garamond/garamond.bdf");
        Font* myFont2 = FontManager::GetFont("GamEncin/Resources/Fonts/Tamzen/Tamzen10x20b.bdf");
        Texture* texture1 = TextureManager::GetTexture("GamEncin/Resources/Textures/test.jpg");
        Texture* texture2 = TextureManager::GetTexture("GamEncin/Resources/Textures/test3.jpg");

        //////////////// camera

        Object& cameraObj = CreateAndUseCameraObject();
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

        ///////////////// test sphere

        Object& playerObj2 = CreateObject();
        playerObj2.SetName("Player Sphere");
        ModelMesh* sphereMesh22 = playerObj2.AddComponent<ModelMesh>();
        sphereMesh22->SetMeshData(MeshBuilder::CreateSphere(1.0f));
        sphereMesh22->SetTexture(texture1);

        RigidBody* playerRB2 = playerObj2.AddComponent<RigidBody>();
        playerRB2->SetDynamic(true);
        playerRB2->SetGravityScale(0);

        Transform* playerTR2 = playerObj2.GetTransform();
        playerTR2->AddPosition(Vector3(2.5f, 0, 0));

        PlayerController* playerController2 = playerObj2.AddComponent<PlayerController>();
    }
};
