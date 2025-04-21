#pragma once 
#include "GamEncin.h"


class MyComponent : public Component
{
public:
    float rotSpeed = 25;

    MyComponent(Object* obj) : Component(obj) {}

    void StartOfSecond() override
    {
        return;

        printf("myComponent StartOfSecond : id : %d, \n", object->GetComponent<Mesh>()->GetMeshData().id);

        Vector3 tempVec = object->GetTransform()->GetLocalPosition();
        printf("\nmyObject localPosition: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = object->GetTransform()->GetLocalRotation();
        printf("myObject localRotation: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = object->GetTransform()->GetLocalScale();
        printf("myObject localScale: %f %f %f\n\n", tempVec.x, tempVec.y, tempVec.z);

        tempVec = object->GetTransform()->GetGlobalPosition();
        printf("myObject globalPosition: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = object->GetTransform()->GetGlobalRotation();
        printf("myObject globalRotation: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = object->GetTransform()->GetGlobalScale();
        printf("myObject globalScale: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);

        tempVec = object->GetTransform()->GetDirection();
        printf("\nmyObject direction: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);

        Matrix4 modelMatrix = object->GetTransform()->GetModelMatrix();

        printf("\nmyObject model matrix\n");
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                printf("%f ", modelMatrix[i][j]);
            }
            printf("\n");
        }
        printf("\n\n");
    }

    void Update() override
    {
        object->GetTransform()->AddRotation(Vector3(1, 3, 1) * Application::GetDeltaTime());

        if(Input::GetKey(Press, C))
        {
            object->GetTransform()->AddRotation(RandomVector3Direction());
        }
    }
};

class CameraController : public Component
{
public:
    float camMoveSpeed = 5;
    float camRotateSpeed = 30;
    float shiftMultiplier = 5;

    Transform* cameraTR = nullptr;
    Camera* camera = nullptr;
    GLFWwindow* window = nullptr;

    vector<Mesh*> meshes;

    CameraController(Object* obj) : Component(obj) {}

    void StartOfSecond() override
    {
        return;

        Vector3 tempVec = cameraTR->GetLocalPosition();
        printf("\ncamera localPosition: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = cameraTR->GetLocalRotation();
        printf("camera localRotation: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = cameraTR->GetLocalScale();
        printf("camera localScale: %f %f %f\n\n", tempVec.x, tempVec.y, tempVec.z);

        tempVec = cameraTR->GetGlobalPosition();
        printf("camera globalPosition: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = cameraTR->GetGlobalRotation();
        printf("camera globalRotation: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = cameraTR->GetGlobalScale();
        printf("camera globalScale: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);

        tempVec = cameraTR->GetDirection();
        printf("\ncamera direction: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);

        Matrix4 modelMatrix = cameraTR->GetModelMatrix();

        printf("\ncam model matrix\n");
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                printf("%f ", modelMatrix[i][j]);
            }
            printf("\n");
        }
        printf("\n\n");

    }

    void Start() override
    {
        cameraTR = object->GetTransform();
        camera = object->GetComponent<Camera>();
        window = Renderer::GetMainWindow();

        vector<Object*> tempObjs = object->GetScene()->FindObjectsWithTag("myObj");

        for(Object* obj : tempObjs)
        {
            meshes.push_back(obj->GetComponent<Mesh>());
        }
    }

    void Update() override
    {
        if(Input::GetKey(Press, KeyCode::Escape) || Input::GetKey(Press, Q))
        {
            Application::Stop(Safe);
            return;
        }

        if(Input::GetKey(Down, F))
        {
            Renderer::SetFullScreen(!Renderer::IsFullScreen());
        }

        if(Input::GetKey(Down, V))
        {
            Renderer::SetVSync(!Renderer::IsVSyncEnabled());
        }

        if(Input::GetKey(Down, R))
        {
            cameraTR->SetLocalPosition(Vector3(0, 0, 0));
        }

        if(Input::GetKey(Press, T))
        {
            if(meshes.size() > 0)
            {
                auto index = RandomRangeInteger(0, meshes.size() - 1);
                auto meshToDelete = meshes[index];
                Renderer::RemoveMesh(meshToDelete);
                meshes.erase(meshes.begin() + index);
            }
        }

        if(Input::GetKey(Down, Y))
        {
            Object& myObject = object->GetScene()->CreateObject();
            Mesh* mesh = myObject.AddComponent<Mesh>();
            mesh->SetMeshData(MeshBuilder::CreateCube());
            mesh->SetMeshTexture(TextureManager::GetTexture(RandomRangeInteger(0, 1) == 0 ? "GamEncin/src/Resources/test3.jpg" : "GamEncin/src/Resources/test.jpg"));
            myObject.SetTag("myObj");
            Renderer::AddMesh(mesh);
            meshes.push_back(mesh);
            myObject.GetTransform()->AddPosition(RandomVector3() * 10);
            myObject.AddComponent<MyComponent>();
            printf("size : %d\n", meshes.size());
        }

        if(Input::GetKey(Down, O))
        {
            camera->SetPerspective(!camera->IsPerspective());
        }

        if(!Input::GetMouseButton(Press, Left))
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            return;
        }

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        Vector3 movement = Input::GetMovementVector();
        Vector2 mouseDelta = Input::GetMousePositionDelta();

        float tempSpeed = Input::GetKey(Press, LeftShift) ? camMoveSpeed * shiftMultiplier : camMoveSpeed;

        camera->AddCameraFOV(-Input::GetMouseScrollDelta());

        Vector3 camPos = cameraTR->GetLocalPosition();
        Vector3 camDir = cameraTR->GetDirection();

        cameraTR->AddPosition(camDir * tempSpeed * movement.y * Application::GetDeltaTime());
        cameraTR->AddPosition(camDir.Cross(Vector3::Up()) * tempSpeed * movement.x * Application::GetDeltaTime());
        cameraTR->AddPosition(Vector3::Up() * tempSpeed * movement.z * Application::GetDeltaTime());

        cameraTR->AddRotation(Vector3(-mouseDelta.y, mouseDelta.x, 0) * camRotateSpeed * Application::GetDeltaTime());
        cameraTR->SetLocalRotation(Vector3(Clamp(cameraTR->GetGlobalRotation().x, -89.0f, 89.0f), cameraTR->GetLocalRotation().y, cameraTR->GetLocalRotation().z));

        if(Input::GetKey(Down, B))
        {
            cameraTR->AddScale(Vector3::One() / 4.0f);
        }
        if(Input::GetKey(Down, V))
        {
            cameraTR->AddScale(Vector3::One() / -4.0f);
        }
    }
};
