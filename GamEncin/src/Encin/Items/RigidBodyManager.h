#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class RigidBody;

    class RigidBodyManager
    {
    public:
        static void AddRigidBody(RigidBody* collider);
        static void RemoveRigidBody(RigidBody* collider);
        static void RemoveAllRigidBodies();

        static void UpdateRigidBodies();
        static void ResolveCollisions();

    private:
        static vector<RigidBody*> colliders;

        RigidBodyManager() = delete;
        RigidBodyManager(const RigidBodyManager&) = delete;
        void operator=(const RigidBodyManager&) = delete;

        static bool CheckForCollision(RigidBody* colliderA, RigidBody* colliderB);
        static void ResolveDynamicVsDynamic(RigidBody* colliderA, RigidBody* colliderB);
        static void ResolveDynamicVsStatic(RigidBody* colliderA, RigidBody* colliderB);
    };
}
