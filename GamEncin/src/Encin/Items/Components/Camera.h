#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/MathYaman.h"

namespace GamEncin
{
    class Camera : public Component
    {
    public:
        Camera(Object* obj);

        float cameraFOV = 45.0f;

        void SetCameraFOV(float fov);
        void SetPerspective(bool value);
        void SetClipPlanes(float nearClip, float farClip);

        bool IsPerspective();

        void UseCamera(unsigned int& viewMatrixVarId, unsigned int& projectionMatrixVarId);

    private:
        bool isPerspective = true;

        float nearClipPlane = 0.1f;
        float farClipPlane = 100.0f;

        Matrix4 projectionMatrix = Matrix4(1.0f);
        Matrix4 viewMatrix = Matrix4(1.0f);
    };
}
