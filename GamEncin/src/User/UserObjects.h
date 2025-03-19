#pragma once 
#include "GamEncin.h"

class myObject : public Pyramid
{
public:
    float speed = 7.5;
    Camera* camera = nullptr;

    myObject() : Pyramid()
    {
        name = "myObject";
        position = Vector3(0, 0, -10);
    }

    void Start() override
    {
        camera = Application::currentScene->renderer->camera;
    }

    void Update() override
    {
        if(!Input::GetMouseButton(MouseLeft))
            return;

        Vector3 movement = Input::GetMovementVector();
        Vector2 mouseDelta = Input::GetMousePositionDelta();

        camera->cameraFOV += Input::GetMouseScrollDelta();

        camera->position += camera->direction * speed * movement.y * Application::deltaTime;
        camera->position += camera->direction.Cross(Vector3::Up()) * speed * movement.x * Application::deltaTime;
        camera->position += Vector3::Up() * speed * movement.z * Application::deltaTime;

        camera->rotation += Vector3(-mouseDelta.y, mouseDelta.x, 0) * 75 * Application::deltaTime;
        printf("Camera Rotation: %f, %f, %f\n", camera->rotation.x, camera->rotation.y, camera->rotation.z);
    }
};
