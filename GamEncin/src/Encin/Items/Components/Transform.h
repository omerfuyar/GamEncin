#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/MathYaman.h"

namespace GamEncin
{
    class Transform : public Component
    {
    public:
        Transform* parent = nullptr;
        vector<Transform*> children;

        Vector3 position = Vector3::Zero();
        Vector3 rotation = Vector3::Zero();
        Vector3 scale = Vector3::One();

        Vector3 localPosition = Vector3::Zero();
        Vector3 localRotation = Vector3::Zero();
        Vector3 localScale = Vector3::One();

        Vector3 direction = Vector3::Forward();

        Matrix4 GetLocalModelMatrix();
        Matrix4 GetWorldModelMatrix();

        Transform(Object* obj);
        void Update() override;

    private:
        Matrix4 localModelMatrix = Matrix4(1.0f);
        Matrix4 worldModelMatrix = Matrix4(1.0f);

        void UpdateProperties();
    };
}
