#pragma once 
#include "GamEncin.h"

class CameraController : public Component
{
public:
    float camMoveSpeed = 5;
    float camRotateSpeed = 30;
    float shiftMultiplier = 5;

    Transform* cameraTR = nullptr;
    Camera* camera = nullptr;
    GLFWwindow* window = nullptr;

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
        cameraTR = object->transform;
        camera = object->GetComponent<Camera>();
        window = Application::GetMainWindow();
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
            Application::Restart();
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

        camera->cameraFOV += -Input::GetMouseScrollDelta();

        Vector3 camPos = cameraTR->GetLocalPosition();
        Vector3 camDir = cameraTR->GetDirection();

        cameraTR->AddPosition(camDir * tempSpeed * movement.y * Application::deltaTime);
        cameraTR->AddPosition(camDir.Cross(Vector3::Up()) * tempSpeed * movement.x * Application::deltaTime);
        cameraTR->AddPosition(Vector3::Up() * tempSpeed * movement.z * Application::deltaTime);

        cameraTR->AddRotation(Vector3(-mouseDelta.y, mouseDelta.x, 0) * camRotateSpeed * Application::deltaTime);
        Vector3 camRot = cameraTR->GetLocalRotation();
        //cameraTR->SetLocalRotation(Vector3(Clamp(cameraTR->GetGlobalRotation().x, -89.0f, 89.0f), camRot.y, camRot.z));

        if(Input::GetKey(Down, B))
        {
            cameraTR->AddScale(Vector3::One() / 4.0f);
        }
        if(Input::GetKey(Down, V))
        {
            cameraTR->AddScale(Vector3::One() / -4.0f);
        }
    }

    void FixUpdate() override
    {
        //if(!Input::GetMouseButton(Press, Left))
        //{
        //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //    return;
        //}
        //
        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //
        //Vector3 movement = Input::GetMovementVector();
        //Vector2 mouseDelta = Input::GetMousePositionDelta();
        //
        //camera->cameraFOV += -Input::GetMouseScrollDelta();
        //
        //cameraTR->position += cameraTR->direction * camMoveSpeed * movement.y;
        //cameraTR->position += cameraTR->direction.Cross(Vector3::Up()) * camMoveSpeed * movement.x;
        //cameraTR->position += Vector3::Up() * camMoveSpeed * movement.z;
        //
        //cameraTR->rotation += Vector3(-mouseDelta.y, mouseDelta.x, 0) * camRotateSpeed;
        //cameraTR->rotation.x = Clamp(cameraTR->rotation.x, -89.0f, 89.0f);
    }
};

class MyComponent : public Component
{
public:
    float rotSpeed = 7.5;

    MyComponent(Object* obj) : Component(obj) {}

    void StartOfSecond() override
    {
        return;

        Vector3 tempVec = object->transform->GetLocalPosition();
        printf("\nmyObject localPosition: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = object->transform->GetLocalRotation();
        printf("myObject localRotation: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = object->transform->GetLocalScale();
        printf("myObject localScale: %f %f %f\n\n", tempVec.x, tempVec.y, tempVec.z);

        tempVec = object->transform->GetGlobalPosition();
        printf("myObject globalPosition: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = object->transform->GetGlobalRotation();
        printf("myObject globalRotation: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);
        tempVec = object->transform->GetGlobalScale();
        printf("myObject globalScale: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);

        tempVec = object->transform->GetDirection();
        printf("\nmyObject direction: %f %f %f\n", tempVec.x, tempVec.y, tempVec.z);

        Matrix4 modelMatrix = object->transform->GetModelMatrix();

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

    void Update()override
    {
        if(Input::GetKey(Press, C))
        {
            object->transform->AddRotation(Vector3::One() * rotSpeed * Application::deltaTime);
        }
    }
};
