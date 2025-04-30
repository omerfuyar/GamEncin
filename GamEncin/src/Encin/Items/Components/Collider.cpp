#include "Encin/Encin.h"

namespace GamEncin
{
#pragma region Collider

    Collider::Collider(Object* obj) : Component(obj)
    {

    }

    void Collider::SetTrigger(bool isTrigger)
    {
        this->isTrigger = isTrigger;
    }

    bool Collider::IsTrigger()
    {
        return isTrigger;
    }

    RigidBody* const Collider::GetRigidBody()
    {
        return rigidBody;
    }

#pragma endregion

#pragma region SphereCollider

    SphereCollider::SphereCollider(Object* obj) : Collider(obj)
    {

    }

    float SphereCollider::GetRadius()
    {
        return radius;
    }

#pragma endregion

#pragma region BoxCollider

    BoxCollider::BoxCollider(Object* obj) : Collider(obj)
    {
    }

    Vector3 BoxCollider::GetSize()
    {
        return size;
    }

    void BoxCollider::SetSize(Vector3 size)
    {
        this->size = size;
    }

#pragma endregion
}
