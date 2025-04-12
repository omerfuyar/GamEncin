#include "Encin/Encin.h"

namespace GamEncin
{
    Camera::Camera(Object* obj) :Component(obj)
    {
    }

    void Camera::SetFOV(float fov)
    {
        cameraFOV = Clamp(fov, 0, 180);
    }

    void Camera::UseCamera(unsigned int& transformMatrixLocation)
    {
        Vector3 position = object->transform->position;
        Vector3 direction = object->transform->direction;
        Vector2Int size = Renderer::GetMainWindowSize();

        viewMatrix = glm::lookAt(position.ToGLMvec3(),
                                 (position + direction).ToGLMvec3(),
                                 Vector3::Up().ToGLMvec3());

        //TODO Deg2Rad may be cause problems with namespace
        perspectiveMatrix = glm::perspective(Deg2Rad(cameraFOV), (float) size.x / (float) size.y, 0.1f, 100.0f);

        glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix * viewMatrix));
        //value_ptr : &mat4[0][0] address of first element of the matrix
    }
}
