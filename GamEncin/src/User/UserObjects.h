#pragma once 
#include "GamEncin.h"

class CameraController : public Object
{
    float camSpeed = 7.5,
        camRotateSpeed = 75,
        cameraLookLimit = 30;
    Camera* camera = nullptr;
    GLFWwindow* window = nullptr;

    void Start() override
    {
        camera = Application::currentScene->renderer->camera;
        window = Application::currentScene->renderer->window;
    }

    void Update() override
    {

        if(Input::IsGamepadConnected(0))
        {
            camera->direction = Vector3(Input::GetGamepadRightStick(0).Normalized(), -1);
            camera->rotation = Vector3(-Input::GetGamepadRightStick(0).y * cameraLookLimit, Input::GetGamepadRightStick(0).x * cameraLookLimit - 90, 0);
        }

        if(!Input::GetMouseButton(Pressed, Left))
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            return;
        }

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        Vector3 movement = Input::GetMovementVector();
        Vector2 mouseDelta = Input::GetMousePositionDelta();

        camera->cameraFOV += -Input::GetMouseScrollDelta();

        camera->position += camera->direction * camSpeed * movement.y * Application::deltaTime;
        camera->position += camera->direction.Cross(Vector3::Up()) * camSpeed * movement.x * Application::deltaTime;
        camera->position += Vector3::Up() * camSpeed * movement.z * Application::deltaTime;

        camera->rotation += Vector3(-mouseDelta.y, mouseDelta.x, 0) * camRotateSpeed * Application::deltaTime;
        camera->rotation.x = Clamp(camera->rotation.x, -89.0f, 89.0f);
    }
};

class myObject : public Cube
{
public:
    float rotSpeed = 75;

    myObject() : Cube(1)
    {
        name = "myObject";
    }

    void Update()override
    {
        rotation += Vector3::One() * rotSpeed * Application::deltaTime;
    }
};
