#include "Encin/Encin.h"

namespace GamEncin
{
    Collider::Collider(Object* obj) : Component(obj)
    {

    }

    void Collider::SetTrigger(bool isTrigger)
    {
        this->isTrigger = isTrigger;
    }

    bool const Collider::IsTrigger()
    {
        return isTrigger;
    }

    RigidBody* const Collider::GetRigidBody()
    {
        return rigidBody;
    }

    ///

    SphereCollider::SphereCollider(Object* obj) : Collider(obj)
    {

    }

    float const SphereCollider::GetRadius()
    {
        return radius;
    }

    /////

    BoxCollider::BoxCollider(Object* obj) : Collider(obj)
    {
    }

    Vector3 const BoxCollider::GetSize()
    {
        return size;
    }

    void BoxCollider::SetSize(Vector3 size)
    {
        this->size = size;
    }
}
