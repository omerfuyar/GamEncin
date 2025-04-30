#pragma once
#include "Encin/Items/Components/Component.h"

namespace GamEncin
{
    class Transform : public Component
    {
    public:
        Transform(Object* obj);

        void SetParent(Transform* newParent);
        void SetLocalPosition(Vector3 newLocalPosition);
        void AddPosition(Vector3 positionToAdd);
        void SetLocalRotation(Vector3 newLocalRotation);
        void AddRotation(Vector3 rotationToAdd);
        void SetLocalScale(Vector3 newLocalScale);
        void AddScale(Vector3 scaleToAdd);

        Transform* const GetParent();
        Vector3 GetLocalPosition();
        Vector3 GetGlobalPosition();
        Vector3 GetLocalRotation();
        Vector3 GetGlobalRotation();
        Vector3 GetLocalScale();
        Vector3 GetGlobalScale();
        Vector3 GetDirection();

        Matrix4 GetModelMatrix();

    private:
        Transform* parent = nullptr;

        Matrix4 modelMatrix = Matrix4(1.0f);

        Vector3 position = Vector3::Zero();
        Vector3 rotation = Vector3::Zero();
        Vector3 scale = Vector3::One();

        Vector3 localPosition = Vector3::Zero();
        Vector3 localRotation = Vector3::Zero();
        Vector3 localScale = Vector3::One();

        Vector3 direction = Vector3::Forward();
    };
}
