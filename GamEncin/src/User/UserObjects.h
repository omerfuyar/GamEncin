#pragma once 
#include "GamEncin.h"

class CameraController : public Component
{
public:
    float camMoveSpeed = 5,
        camRotateSpeed = 50,
        cameraLookLimit = 30;

    Transform* cameraTR = nullptr;
    Camera* camera = nullptr;
    GLFWwindow* window = nullptr;

    CameraController(Object* object) : Component(object) {}

    void Start() override
    {
        cameraTR = object->transform;
        camera = object->GetComponent<Camera>();
        window = Application::currentScene->renderer->window;
    }

    void Update() override
    {
        if(!Input::GetMouseButton(Pressed, Left))
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Vector2 vec;
            vec.Normalized() += 5;
            vec.Normalized() + 5;
            return;
        }

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        Vector3 movement = Input::GetMovementVector();
        Vector2 mouseDelta = Input::GetMousePositionDelta();

        camera->cameraFOV += -Input::GetMouseScrollDelta();

        cameraTR->position += cameraTR->direction * camMoveSpeed * movement.y * Application::deltaTime;
        cameraTR->position += cameraTR->direction.Cross(Vector3::Up()) * camMoveSpeed * movement.x * Application::deltaTime;
        cameraTR->position += Vector3::Up() * camMoveSpeed * movement.z * Application::deltaTime;

        cameraTR->rotation += Vector3(-mouseDelta.y, mouseDelta.x, 0) * camRotateSpeed * Application::deltaTime;
        cameraTR->rotation.x = Clamp(cameraTR->rotation.x, -89.0f, 89.0f);
    }
};

class MyComponent : public Component
{
public:
    float rotSpeed = 75;

    MyComponent(Object* object) : Component(object)
    {
    }

    void Update()override
    {
        object->transform->rotation += Vector3::Up() * rotSpeed * Application::deltaTime;
        //object->transform->position += Vector3::Forward() * Application::deltaTime;
    }
};
