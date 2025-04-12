#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/MathYaman.h"

#include "GLAD/glad.h"

namespace GamEncin
{
    class Camera : public Component
    {
    public:
        Camera(Object* obj);

        GLfloat cameraFOV = 45.0f;

        void SetFOV(float fov);
        void UseCamera(unsigned int& transformMatrixLocation);

    private:
        glm::mat4 perspectiveMatrix = glm::mat4(1.0f),
            viewMatrix = glm::mat4(1.0f);
    };
}
