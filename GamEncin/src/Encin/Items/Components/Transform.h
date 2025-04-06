#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/MathYaman.h"

namespace GamEncin
{
    class Transform : public Component
    {
    public:
        Transform(Object* object) : Component(object) {};

        Transform* parent = nullptr;
        vector<Transform*> children;

        Vector3 position = Vector3::Zero(),
            rotation = Vector3::Zero(),
            scale = Vector3::One(),
            localPosition = Vector3::Zero(),
            localRotation = Vector3::Zero(),
            localScale = Vector3::One(),
            direction = Vector3::Forward();

        void Update() override;

    private:
        void UpdateProperties();
    };
}
