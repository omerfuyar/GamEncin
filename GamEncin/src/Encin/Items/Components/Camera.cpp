#include "Encin/Encin.h"

namespace GamEncin
{
    Camera::Camera(Object* obj) :Component(obj) {}

    void Camera::SetCameraFOV(float fov)
    {
        cameraFOV = Clamp(fov, 1, 179);
    }

    void Camera::AddCameraFOV(float fov)
    {
        cameraFOV = Clamp(cameraFOV + fov, 1, 179);
    }

    void Camera::SetOrthographicSize(float size)
    {
        orthographicSize = size;
    }

    void Camera::AddOrthographicSize(float size)
    {
        orthographicSize += size;
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

    bool const Camera::IsPerspective()
    {
        return isPerspective;
    }

    float const Camera::GetCameraFOV()
    {
        return cameraFOV;
    }

    float const Camera::GetOrthographicSize()
    {
        return orthographicSize;
    }

    void Camera::UseCamera(unsigned int& viewMatrixVarId, unsigned int& projectionMatrixVarId)
    {
        glm::vec3 position = object->GetTransform()->GetGlobalPosition().ToGLMvec3();
        glm::vec3 direction = object->GetTransform()->GetDirection().ToGLMvec3();
        Vector2Int size = Renderer::GetMainWindowSize();

        viewMatrix = glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));

        float sizeX = (float) size.x * orthographicSize / 1000.0f;
        float sizeY = (float) size.y * orthographicSize / 1000.0f;

        projectionMatrix = isPerspective ?
            glm::perspective(Deg2Rad(cameraFOV), sizeX / sizeY, nearClipPlane, farClipPlane) :
            glm::ortho(-sizeX, sizeX, -sizeY, sizeY, nearClipPlane, farClipPlane);

        glUniformMatrix4fv(viewMatrixVarId, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniformMatrix4fv(projectionMatrixVarId, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        //value_ptr : &mat4[0][0] address of first element of the matrix
    }
}
