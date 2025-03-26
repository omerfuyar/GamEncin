#pragma once 
#include "GamEncin.h"

class myObject : public Simit
{
public:
    float camSpeed = 7.5,
        rotSpeed = 75;
    Camera* camera = nullptr;
    GLFWwindow* window = nullptr;

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

        //if(!Input::GetMouseButton(Pressed, Left))
        //{
        //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        //    return;
        //}

        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if(Input::GetGamePadButton(0, Pressed, A_Cross))
            printf("ID: %d, A_Cross is pressed\n", 0);
        if(Input::GetGamePadButton(0, Down, B_Circle))
            printf("ID: %d, B_Circle is Down\n", 0);
        if(Input::GetGamePadButton(0, Up, Y_Triangle))
            printf("ID: %d, Y_Triangle is Up\n", 0);

        float val = Input::GetGamePadLeftTrigger(0);
        if(val > 0)
            printf("ID: %d, Left Trigger: %f\n", 0, val);

        Vector2 stick = Input::GetGamePadRightStick(0);
        if(stick.GetMagnitude() > 0.1)
            printf("ID: %d, Left Stick: %f, %f\n", 0, stick.x, stick.y);

        Vector3 movement = Input::GetMovementVector();
        Vector2 mouseDelta = Input::GetMousePositionDelta();

        camera->cameraFOV += -Input::GetMouseScrollDelta();

        camera->position += camera->direction * camSpeed * movement.y * Application::deltaTime;
        camera->position += camera->direction.Cross(Vector3::Up()) * camSpeed * movement.x * Application::deltaTime;
        camera->position += Vector3::Up() * camSpeed * movement.z * Application::deltaTime;

        //camera->rotation += Vector3(-mouseDelta.y, mouseDelta.x, 0) * 75 * Application::deltaTime;
        camera->rotation.x = Clamp(camera->rotation.x, -89.0f, 89.0f);
    }
};
