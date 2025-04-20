#include "Encin/Encin.h"

namespace GamEncin
{
    Camera::Camera(Object* obj) :Component(obj) {}

    void Camera::SetCameraFOV(float fov)
    {
        cameraFOV = Clamp(fov, 1, 179);
    }

    void Camera::SetOrthographicSize(float size)
    {
        orthographicSize = size;
    }

    void Camera::SetPerspective(bool value)
    {
        isPerspective = value;
    }

    void Camera::SetClipPlanes(float nearClip, float farClip)
    {
        nearClipPlane = nearClip;
        farClipPlane = farClip;
    }

    bool Camera::IsPerspective()
    {
        return isPerspective;
    }

    void Camera::UseCamera(unsigned int& viewMatrixVarId, unsigned int& projectionMatrixVarId)
    {
        glm::vec3 position = object->transform->GetGlobalPosition().ToGLMvec3();
        glm::vec3 direction = object->transform->GetDirection().ToGLMvec3();
        Vector2Int size = Renderer::GetMainWindowSize();

        //viewMatrix = glm::inverse(object->transform->GetWorldModelMatrix());
        viewMatrix = glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));

        float sizeX = (float) size.x / 2.0f;
        float sizeY = (float) size.y / 2.0f;

        projectionMatrix = isPerspective ?
            glm::perspective(Deg2Rad(cameraFOV), sizeX / sizeY, nearClipPlane, farClipPlane) :
            glm::ortho(-sizeX * orthographicSize, sizeX * orthographicSize, -sizeY * orthographicSize, sizeY * orthographicSize, nearClipPlane, farClipPlane);

        glUniformMatrix4fv(viewMatrixVarId, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrixVarId, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        //value_ptr : &mat4[0][0] address of first element of the matrix
    }
}
