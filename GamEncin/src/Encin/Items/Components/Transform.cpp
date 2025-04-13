#include "Encin/Encin.h"

namespace GamEncin
{
    Transform::Transform(Object* obj) :Component(obj) {}

    Matrix4 Transform::GetLocalModelMatrix()
    {
        Matrix4 translate = glm::translate(position.ToGLMvec3());
        Matrix4 RotateX = glm::rotate(Deg2Rad(rotation.x), glm::vec3(1, 0, 0));
        Matrix4 RotateY = glm::rotate(Deg2Rad(rotation.y), glm::vec3(0, 1, 0));
        Matrix4 RotateZ = glm::rotate(Deg2Rad(rotation.z), glm::vec3(0, 0, 1));
        Matrix4 Rotate = RotateZ * RotateY * RotateX;
        Matrix4 Scale = glm::scale(scale.ToGLMvec3());

        localModelMatrix = translate * Rotate * Scale;

        return localModelMatrix;
    }

    Matrix4 Transform::GetWorldModelMatrix()
    {
        Matrix4 localMat = GetLocalModelMatrix();

        if(parent)
        {
            worldModelMatrix = parent->GetWorldModelMatrix() * localMat;
        }
        else
        {
            worldModelMatrix = localMat;
        }

        return worldModelMatrix;
    }

    void Transform::UpdateProperties()
    {
        GetWorldModelMatrix();

        //TODO fix direction
        direction.x = CosDeg(rotation.x) * CosDeg(rotation.y);
        direction.y = SinDeg(rotation.x);
        direction.z = CosDeg(rotation.x) * SinDeg(rotation.y);
        direction.Normalize();
    }

    void Transform::Update()
    {
        UpdateProperties();
    }
}
