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
        printf("camera position: %f %f %f\n", cameraTR->position.x, cameraTR->position.y, cameraTR->position.z);
        printf("camera rotation: %f %f %f\n", cameraTR->rotation.x, cameraTR->rotation.y, cameraTR->rotation.z);
        printf("camera scale: %f %f %f\n", cameraTR->scale.x, cameraTR->scale.y, cameraTR->scale.z);
        printf("camera direction: %f %f %f\n", cameraTR->direction.x, cameraTR->direction.y, cameraTR->direction.z);

        Matrix4 modelMatrix = cameraTR->GetWorldModelMatrix();

        printf("cam model matrix\n\n");
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
        if(Input::GetKey(Pressed, KeyCode::Escape) || Input::GetKey(Pressed, Q))
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

        if(!Input::GetMouseButton(Pressed, Left))
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            return;
        }

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        Vector3 movement = Input::GetMovementVector();
        Vector2 mouseDelta = Input::GetMousePositionDelta();

        float tempSpeed = Input::GetKey(Pressed, LeftShift) ? camMoveSpeed * shiftMultiplier : camMoveSpeed;

        camera->cameraFOV += -Input::GetMouseScrollDelta();

        cameraTR->position += cameraTR->direction * tempSpeed * movement.y * Application::deltaTime;
        cameraTR->position += cameraTR->direction.Cross(Vector3::Up()) * tempSpeed * movement.x * Application::deltaTime;
        cameraTR->position += Vector3::Up() * tempSpeed * movement.z * Application::deltaTime;

        cameraTR->rotation += Vector3(-mouseDelta.y, mouseDelta.x, 0) * camRotateSpeed * Application::deltaTime;
        cameraTR->rotation.x = Clamp(cameraTR->rotation.x, -89.0f, 89.0f);
    }

    void FixUpdate() override
    {
        //if(!Input::GetMouseButton(Pressed, Left))
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
    float rotSpeed = 75;

    MyComponent(Object* obj) : Component(obj) {}

    void Update()override
    {
        //object->transform->rotation += Vector3::Up() * rotSpeed * Application::deltaTime;
        //object->transform->position += Vector3::Forward() * Application::deltaTime;
    }
};
