#include "Encin/Encin.h"

namespace GamEncin
{
    Transform::Transform(Object* obj) : Component(obj) {}

    void Transform::SetParent(Transform* newParent)
    {
        if(newParent == this)
        {
            Application::PrintLog(idoiterr, "Transform trying to set as parent is itself");
            return;
        }

        newParent = parent;

        //if(parent)
        //{
        //    auto child = std::find(parent->children.begin(), parent->children.end(), this);
        //
        //    if(child != parent->children.end())
        //    {
        //        parent->children.erase(child);
        //    }
        //    else
        //    {
        //        Application::PrintLog(ElementCouldNotFindErr, "Transform trying to remove parent is not in children");
        //    }
        //}
        //
        //parent = newParent;
        //
        //if(parent)
        //{
        //    auto child = std::find(parent->children.begin(), parent->children.end(), this);
        //
        //    if(child == parent->children.end())
        //    {
        //        parent->children.push_back(this);
        //    }
        //    else
        //    {
        //        Application::PrintLog(ElementDuplicationErr, "Transform trying to add itself as child is already in children");
        //    }
        //}
    }

    void Transform::SetLocalPosition(Vector3 newLocalPosition)
    {
        localPosition = newLocalPosition;
    }

    void Transform::AddPosition(Vector3 positionToAdd)
    {
        SetLocalPosition(localPosition + positionToAdd);
    }

    void Transform::SetLocalRotation(Vector3 newLocalRotation)
    {
        localRotation = newLocalRotation;
    }

    void Transform::AddRotation(Vector3 rotationToAdd)
    {
        SetLocalRotation(localRotation + rotationToAdd);
    }

    void Transform::SetLocalScale(Vector3 newLocalScale)
    {
        localScale = newLocalScale;
    }

    void Transform::AddScale(Vector3 scaleToAdd)
    {
        SetLocalScale(localScale + scaleToAdd);
    }

    Transform* const Transform::GetParent()
    {
        return parent;
    }

    Vector3 Transform::GetLocalPosition()
    {
        return localPosition;
    }

    Vector3 Transform::GetGlobalPosition()
    {
        position = parent ? parent->GetGlobalPosition() + localPosition : localPosition;

        //position = Vector3(glm::vec3(GetModelMatrix()[3]));

        return position;
    }

    Vector3 Transform::GetLocalRotation()
    {
        return localRotation;
    }

    Vector3 Transform::GetGlobalRotation()
    {
        rotation = parent ? parent->GetGlobalRotation() + localRotation : localRotation;

        //glm::mat3 rotationMatrix = glm::mat3(GetModelMatrix());
        //
        //rotationMatrix[0] = glm::normalize(rotationMatrix[0]);
        //rotationMatrix[1] = glm::normalize(rotationMatrix[1]);
        //rotationMatrix[2] = glm::normalize(rotationMatrix[2]);
        //
        //rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(rotationMatrix)));

        return rotation;
    }

    Vector3 Transform::GetLocalScale()
    {
        return localScale;
    }

    Vector3 Transform::GetGlobalScale()
    {
        scale = parent ? parent->GetGlobalScale() * localScale : localScale;

        //Matrix4 tempMatrix = GetModelMatrix();
        //
        //scale.x = glm::length(glm::vec3(tempMatrix[0]));
        //scale.y = glm::length(glm::vec3(tempMatrix[1]));
        //scale.z = glm::length(glm::vec3(tempMatrix[2]));

        return scale;
    }

    Vector3 Transform::GetDirection()
    {
        //direction = glm::normalize(glm::vec3(GetModelMatrix()[2]));

        //Matrix4 tempMatrix = GetModelMatrix();
        //tempMatrix[0] /= scale.x;
        //tempMatrix[1] /= scale.y;
        //tempMatrix[2] /= scale.z;
        //
        //direction = glm::normalize(glm::vec3(tempMatrix[2]));

        Vector3 tempRot = GetGlobalRotation();

        direction = Vector3(CosDeg(tempRot.x) * CosDeg(tempRot.y),
                            SinDeg(tempRot.x),
                            CosDeg(tempRot.x) * SinDeg(tempRot.y)).Normalize();

        return direction;
    }

    Matrix4* const Transform::GetModelMatrix()
    {
        Vector3 tempPos = GetGlobalPosition();
        Vector3 tempRot = GetGlobalRotation();
        Vector3 tempScale = GetGlobalScale();

        Matrix4 Translate = glm::translate(tempPos.ToGLMvec3());

        Matrix4 RotateX = glm::rotate(Deg2Rad(tempRot.x), glm::vec3(1, 0, 0));
        Matrix4 RotateY = glm::rotate(Deg2Rad(tempRot.y), glm::vec3(0, 1, 0));
        Matrix4 RotateZ = glm::rotate(Deg2Rad(tempRot.z), glm::vec3(0, 0, 1));
        Matrix4 Rotate = RotateZ * RotateY * RotateX;

        Matrix4 Scale = glm::scale(tempScale.ToGLMvec3());

        modelMatrix = Translate * Rotate * Scale;

        if(parent)
        {
            modelMatrix = *parent->GetModelMatrix() * modelMatrix;
        }

        return &modelMatrix;
    }
}
