#pragma once 
#include "GamEncin.h"

class myObject : public Pyramid
{
public:
    float speed = 7.5;

    myObject() : Pyramid()
    {
        position = Vector3(0, 0, -10);
    }

    void Update() override
    {
        if(Input::GetKey(KeyCode::W))
            position += Vector3(0, speed * Application::deltaTime, 0);
        if(Input::GetKey(KeyCode::A))
            position += Vector3(-speed * Application::deltaTime, 0, 0);
        if(Input::GetKey(KeyCode::S))
            position += Vector3(0, -speed * Application::deltaTime, 0);
        if(Input::GetKey(KeyCode::D))
            position += Vector3(speed * Application::deltaTime, 0, 0);

        if(Input::GetKey(KeyCode::Space))
            printf("Space key press");
        if(Input::GetKeyDown(KeyCode::Enter))
            printf("Enter key down");
    }
};
