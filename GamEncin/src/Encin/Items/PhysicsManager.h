#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class RigidBody;
    class Scene;

    class PhysicsManager
    {
    public:
        // shouldn't be used by user
        static void AddRigidBodiesInScene(Scene* scene);
        // shouldn't be used by user
        static void ClearRigidBodies();
        static void AddRigidBody(RigidBody* collider);
        static void RemoveRigidBody(RigidBody* collider);
        static void RemoveAllRigidBodies();

        static void UpdateRigidBodies();
        static void ResolveCollisions();

    private:
        static vector<RigidBody*> colliders;

        PhysicsManager() = delete;
        PhysicsManager(const PhysicsManager&) = delete;
        void operator=(const PhysicsManager&) = delete;

        static bool CheckForCollision(RigidBody* colliderA, RigidBody* colliderB);
        static void ResolveDynamicVsDynamic(RigidBody* colliderA, RigidBody* colliderB);
        static void ResolveDynamicVsStatic(RigidBody* colliderA, RigidBody* colliderB);
    };
}
