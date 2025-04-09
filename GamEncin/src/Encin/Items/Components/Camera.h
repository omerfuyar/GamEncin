#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/MathYaman.h"

#include "GLAD/glad.h"

namespace GamEncin
{
    class Camera : public Component
    {
    public:
        Camera(Object* object) : Component(object) {};
        Camera(Object* object, Vector2Int size, float FOV);
        ~Camera() = default;

        GLfloat cameraFOV = 0.0f;
        Vector2Int size = Vector2Int(1080, 1080);

        void UseCamera(unsigned int& transformMatrixLocation);

    private:
        glm::mat4 perspectiveMatrix = glm::mat4(1.0f),
            viewMatrix = glm::mat4(1.0f);
    };
}
