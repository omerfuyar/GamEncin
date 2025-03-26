#pragma once 
#include "GamEncin.h"

class myObject : public Simit
{
public:
    float camSpeed = 7.5,
        rotSpeed = 75;
    Camera* camera = nullptr;
    GLFWwindow* window = nullptr;
    bool a;

    myObject() : Simit(1, 0.5, 200)
    {
        name = "myObject";
        position = Vector3(0, 0, -10);
    }

    void Start() override
    {
        camera = Application::currentScene->renderer->camera;
        window = Application::currentScene->renderer->window;
    }

    void Update() override
    {
        rotation += Vector3::One() * rotSpeed * Application::deltaTime;

        if(!Input::GetKey(Pressed, MouseLeft))
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

        camera->rotation += Vector3(-mouseDelta.y, mouseDelta.x, 0) * 75 * Application::deltaTime;
        camera->rotation.x = Clamp(camera->rotation.x, -89.0f, 89.0f);
    }
};
