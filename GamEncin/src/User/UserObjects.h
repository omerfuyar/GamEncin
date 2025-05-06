#pragma once 
#include "GamEncin.h"

class PlayerController : public Component
{
public:
    float moveSpeed = 2;
    Transform* playerTR = nullptr;
    RigidBody* playerRB = nullptr;

    PlayerController(Object* obj) : Component(obj) {}

    void Awake() override
    {
        playerTR = object->GetTransform();
        playerRB = object->GetComponent<RigidBody>();
    }

    void StartOfSecond() override
    {
    }

    void Update() override
    {
        Vector3 input = Input::GetMovementVector();
        playerTR->AddPosition((Vector2) input * moveSpeed * Application::GetDeltaTime());

        if(Input::GetKey(Down, T))
        {
            playerRB->SetTrigger(!playerRB->IsTrigger());
        }
    }

    void OnCollisionEnter(RigidBody* rb) override
    {
        printf("Collision Enter: %s\n", rb->GetOwnerObject()->GetName().c_str());
    }

    void OnCollisionStay(RigidBody* rb) override
    {
        printf("Collision Stay: %s\n", rb->GetOwnerObject()->GetName().c_str());
    }

    void OnCollisionExit(RigidBody* rb) override
    {
        printf("Collision Exit: %s\n", rb->GetOwnerObject()->GetName().c_str());
    }

    void OnTriggerEnter(RigidBody* rb) override
    {
        printf("Trigger Enter: %s\n", rb->GetOwnerObject()->GetName().c_str());
    }

    void OnTriggerStay(RigidBody* rb) override
    {
        printf("Trigger Stay: %s\n", rb->GetOwnerObject()->GetName().c_str());
    }

    void OnTriggerExit(RigidBody* rb) override
    {
        printf("Trigger Exit: %s\n", rb->GetOwnerObject()->GetName().c_str());
    }
};

class CameraController : public Component
{
public:
    float camMoveSpeed = 5;
    float camRotateSpeed = 30;
    float shiftMultiplier = 5;
    bool lockMovement = false;

    Transform* cameraTR = nullptr;
    Camera* camera = nullptr;
    GLFWwindow* window = nullptr;

    CameraController(Object* obj) : Component(obj) {}

    void StartOfSecond() override
    {
    }

    void Start() override
    {
        cameraTR = object->GetTransform();
        camera = object->GetComponent<Camera>();
        window = Renderer::GetMainWindow();
    }

    void LockMovement(bool value)
    {
        lockMovement = value;
    }

    void Update() override
    {

        if(Input::GetKey(Press, KeyCode::Escape) || Input::GetKey(Press, Q))
        {
            Application::Stop(Safe, "Application ended by user.");
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

        if(Input::GetKey(Down, L))
        {
            lockMovement = !lockMovement;
        }

        if(Input::GetKey(Down, Y))
        {

        }

        if(Input::GetKey(Down, O))
        {
            camera->SetPerspective(!camera->IsPerspective());
        }

        if(lockMovement) return;

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
        camera->AddOrthographicSize(-Input::GetMouseScrollDelta());

        Vector3 camPos = cameraTR->GetLocalPosition();
        Vector3 camDir = cameraTR->GetDirection();

        cameraTR->AddPosition(camDir * tempSpeed * movement.y * Application::GetDeltaTime());
        cameraTR->AddPosition(camDir.Cross(Vector3::Up()) * tempSpeed * movement.x * Application::GetDeltaTime());
        cameraTR->AddPosition(Vector3::Up() * tempSpeed * movement.z * Application::GetDeltaTime());

        cameraTR->AddRotation(Vector3(-mouseDelta.y, mouseDelta.x, 0) * camRotateSpeed * Application::GetDeltaTime());
        cameraTR->SetLocalRotation(Vector3(Clamp(cameraTR->GetGlobalRotation().x, -89.0f, 89.0f), cameraTR->GetLocalRotation().y, cameraTR->GetLocalRotation().z));
    }
};
