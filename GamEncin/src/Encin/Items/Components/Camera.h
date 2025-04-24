#pragma once
#include "Encin/Items/Components/Component.h"

namespace GamEncin
{
    class Camera : public Component
    {
    public:
        Camera(Object* obj);

        void SetCameraFOV(float fov);
        void AddCameraFOV(float fov);
        void SetOrthographicSize(float size);
        void AddOrthographicSize(float size);
        void SetPerspective(bool value);
        void SetClipPlanes(float nearClip, float farClip);

        bool const IsPerspective();
        float const GetCameraFOV();
        float const GetOrthographicSize();

        void UseCamera(unsigned int& viewMatrixVarId, unsigned int& projectionMatrixVarId);

    private:
        float cameraFOV = 45.0f;
        float orthographicSize = 10.0f;
        bool isPerspective = true;

        float nearClipPlane = 0.1f;
        float farClipPlane = 100.0f;

        Matrix4 projectionMatrix = Matrix4(1.0f);
        Matrix4 viewMatrix = Matrix4(1.0f);
    };
}
