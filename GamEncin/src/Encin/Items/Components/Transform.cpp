#include "Encin/Encin.h"

namespace GamEncin
{
    Transform::Transform(Object* obj) : Component(obj) {}

    Transform::~Transform()
    {
        printf("transform destructor %p, name %s\n", this, object->GetName().c_str());
        for(Transform* child : children)
        {
            if(!child)
            {
                Application::PrintLog(NullPointerErr, "Child transform trying to delete is null");
            }

            printf("transform destructor\n : deleting %p : this %p", child, this);
            fflush(stdout);
            delete child->GetOwnerObject();
        }
    }

    void Transform::SetParent(Transform* newParent)
    {
        if(newParent == this)
        {
            Application::PrintLog(idoiterr, "Transform trying to set as parent is itself");
            return;
        }

        if(parent)
        {
            auto obj = std::find_if(parent->children.begin(), parent->children.end(), [this](Transform* child)
                                    { return child == this; });

            if(obj != parent->children.end())
            {
                parent->children.erase(obj);
            }
        }

        if(newParent)
        {
            newParent->children.push_back(this);
        }

        parent = newParent;
    }

    void Transform::RemoveChild(Transform* child)
    {
        if(child == this)
        {
            Application::PrintLog(idoiterr, "Transform trying to remove itself as child");
            return;
        }

        auto obj = std::find_if(children.begin(), children.end(), [child](Transform* c)
                                { return c == child; });

        if(obj == children.end())
        {
            Application::PrintLog(ElementCouldNotFindErr, "Child transform couldn't find in the parent");
        }

        children.erase(obj);
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

    void Transform::AddScale(Vector3 scaleToMult)
    {
        SetLocalScale(localScale * scaleToMult);
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

        // position = Vector3(glm::vec3(GetModelMatrix()[3]));

        return position;
    }

    Vector3 Transform::GetLocalRotation()
    {
        return localRotation;
    }

    Vector3 Transform::GetGlobalRotation()
    {
        rotation = parent ? parent->GetGlobalRotation() + localRotation : localRotation;

        // glm::mat3 rotationMatrix = glm::mat3(GetModelMatrix());
        //
        // rotationMatrix[0] = glm::normalize(rotationMatrix[0]);
        // rotationMatrix[1] = glm::normalize(rotationMatrix[1]);
        // rotationMatrix[2] = glm::normalize(rotationMatrix[2]);
        //
        // rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(rotationMatrix)));

        return rotation;
    }

    Vector3 Transform::GetLocalScale()
    {
        return localScale;
    }

    Vector3 Transform::GetGlobalScale()
    {
        scale = parent ? parent->GetGlobalScale() * localScale : localScale;

        // Matrix4 tempMatrix = GetModelMatrix();
        //
        // scale.x = glm::length(glm::vec3(tempMatrix[0]));
        // scale.y = glm::length(glm::vec3(tempMatrix[1]));
        // scale.z = glm::length(glm::vec3(tempMatrix[2]));

        return scale;
    }

    Vector3 Transform::GetDirection()
    {
        // direction = glm::normalize(glm::vec3(GetModelMatrix()[2]));

        // Matrix4 tempMatrix = GetModelMatrix();
        // tempMatrix[0] /= scale.x;
        // tempMatrix[1] /= scale.y;
        // tempMatrix[2] /= scale.z;
        //
        // direction = glm::normalize(glm::vec3(tempMatrix[2]));

        Vector3 tempRot = GetGlobalRotation();

        direction = Vector3(CosDeg(tempRot.x) * CosDeg(tempRot.y),
                            SinDeg(tempRot.x),
                            CosDeg(tempRot.x) * SinDeg(tempRot.y))
            .Normalize();

        return direction;
    }

    Matrix4* const Transform::GetModelMatrix()
    {
        Matrix4 localTranslateMatrix = glm::translate(localPosition.ToGLMvec3());

        Matrix4 localRotateMatrixX = glm::rotate(Deg2Rad(localRotation.x), glm::vec3(1, 0, 0));
        Matrix4 localRotateMatrixY = glm::rotate(Deg2Rad(localRotation.y), glm::vec3(0, 1, 0));
        Matrix4 localRotateMatrixZ = glm::rotate(Deg2Rad(localRotation.z), glm::vec3(0, 0, 1));
        Matrix4 localRotateMatrix = localRotateMatrixZ * localRotateMatrixY * localRotateMatrixX;

        Matrix4 localScaleMatrix = glm::scale(localScale.ToGLMvec3());

        Matrix4 localModelMatrix = localTranslateMatrix * localRotateMatrix * localScaleMatrix;

        modelMatrix = parent ? *(parent->GetModelMatrix()) * localModelMatrix : localModelMatrix;

        return &modelMatrix;
    }
}
