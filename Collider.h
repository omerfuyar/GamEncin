#pragma once
#include "Encin/Items/Components/Component.h"
namespace GamEncin
{
    class Collider :public Component
    {
    public:
        Collider(Object* obj);
        ~Collider();

        void SetTrigger(bool isTrigger);

        bool const IsTrigger();
        RigidBody* const GetRigidBody();

    private:
        bool isTrigger = false;
        RigidBody* rigidBody = nullptr;
    };
}
