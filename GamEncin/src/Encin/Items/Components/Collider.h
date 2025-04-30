#pragma once
#include "Encin/Items/Components/Component.h"
namespace GamEncin
{
    class Collider :public Component
    {
    public:
        Collider(Object* obj);

        void SetTrigger(bool isTrigger);

        bool const IsTrigger();
        RigidBody* const GetRigidBody();

    private:
        bool isTrigger = false;
        RigidBody* rigidBody = nullptr;
    };

    class SphereCollider : public Collider
    {
    public:
        SphereCollider(Object* obj);
        float const GetRadius();

    private:
        float radius = 1.0f;
    };

    class BoxCollider : public Collider
    {
    public:
        BoxCollider(Object* obj);
        Vector3 const GetSize();
        void SetSize(Vector3 size);
    private:
        Vector3 size = Vector3::One();
    };
}
