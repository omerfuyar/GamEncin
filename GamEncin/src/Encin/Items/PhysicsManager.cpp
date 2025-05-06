#include "Encin/Encin.h"

namespace GamEncin
{
    vector<RigidBody*> PhysicsManager::colliders;

    void PhysicsManager::AddRigidBodiesInScene(Scene* scene)
    {
        if(!scene)
        {
            Application::PrintLog(NullPointerErr, "Scene trying to add colliders is null.");
            return;
        }

        vector<RigidBody* > tempColliders = scene->FindComponentsByType<RigidBody>();

        for(auto& obj : tempColliders)
        {
            AddRigidBody(obj);
        }
    }

    void PhysicsManager::ClearRigidBodies()
    {
        for(RigidBody* collider : colliders)
        {
            collider->ClearCollisions();
        }

        colliders.clear();
    }

    void PhysicsManager::AddRigidBody(RigidBody* collider)
    {
        if(!collider)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to add is null.");
            return;
        }

        auto obj = std::find(colliders.begin(), colliders.end(), collider);

        if(obj != colliders.end())
        {
            Application::PrintLog(ElementDuplicationErr, "RigidBody already exists in the collider manager.");
            return;
        }

        colliders.push_back(collider);
    }

    void PhysicsManager::RemoveRigidBody(RigidBody* collider)
    {
        if(!collider)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to remove is null.");
            return;
        }

        auto obj = std::find(colliders.begin(), colliders.end(), collider);

        if(obj == colliders.end())
        {
            Application::PrintLog(ElementCouldNotFindErr, "RigidBody couldn't find in the collider manager.");
            return;
        }

        colliders.erase(obj);
    }

    void PhysicsManager::RemoveAllRigidBodies()
    {
        colliders.clear();
    }

    void PhysicsManager::UpdateRigidBodies()
    {
        for(int i = 0; i < colliders.size(); i++)
        {
            for(int j = i + 1; j < colliders.size(); j++)
            {
                RigidBody* colliderA = colliders[i];
                RigidBody* colliderB = colliders[j];

                if(CheckForCollision(colliderA, colliderB))
                {
                    colliderA->AddCollision(colliderB);
                    colliderB->AddCollision(colliderA);
                }
                else if(CheckForTrigger(colliderA, colliderB))
                {
                    colliderA->AddCollision(colliderB);
                    colliderB->AddCollision(colliderA);
                }
                else
                {
                    colliderA->RemoveCollision(colliderB);
                    colliderB->RemoveCollision(colliderA);
                }
            }
        }
    }

    void PhysicsManager::ResolveCollisions()
    {
        for(RigidBody* body : colliders)
        {
            if(body->IsTrigger()) continue;

            for(RigidBody* otherBody : body->GetCollisions())
            {
                if(otherBody->IsTrigger()) continue;

                if(body->IsDynamic() && otherBody->IsDynamic())
                {
                    ResolveDynamicVsDynamic(body, otherBody);
                }
                else if(body->IsDynamic() && !otherBody->IsDynamic())
                {
                    ResolveDynamicVsStatic(body, otherBody);
                }
                else if(!body->IsDynamic() && otherBody->IsDynamic())
                {
                    ResolveDynamicVsStatic(otherBody, body);
                }
            }
        }
    }

    bool PhysicsManager::CheckForCollision(RigidBody* colliderA, RigidBody* colliderB)
    {
        if(!colliderA || !colliderB)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to check is null.");
            return false;
        }

        if(colliderA->IsTrigger() || colliderB->IsTrigger()) return false;

        return (colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition()).GetMagnitude() < (colliderA->GetRigidBodyRadius() + colliderB->GetRigidBodyRadius());
    }

    bool PhysicsManager::CheckForTrigger(RigidBody* colliderA, RigidBody* colliderB)
    {
        if(!colliderA || !colliderB)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to check is null.");
            return false;
        }

        return (colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition()).GetMagnitude() < (colliderA->GetRigidBodyRadius() + colliderB->GetRigidBodyRadius());
    }

    void PhysicsManager::ResolveDynamicVsDynamic(RigidBody* colliderA, RigidBody* colliderB)
    {
        Vector3 distance = colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition();

        float overlap = (colliderA->GetRigidBodyRadius() + colliderB->GetRigidBodyRadius()) - distance.GetMagnitude();

        colliderA->GetOwnerObject()->GetTransform()->AddPosition(distance.Normalized() * -overlap / 2);
        colliderB->GetOwnerObject()->GetTransform()->AddPosition(distance.Normalized() * overlap / 2);

        distance = colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition();

        Vector3 velocityA = colliderA->GetVelocity();
        Vector3 velocityB = colliderB->GetVelocity();

        float massA = colliderA->GetMass();
        float massB = colliderB->GetMass();

        float n = (2 * (velocityB - velocityA).Dot(distance)) / (Square(distance.GetMagnitude()) * (massA + massB));

        Vector3 resolveVectorA = distance * massB * n;
        Vector3 resolveVectorB = distance * massA * -n;

        colliderA->AddVelocity(resolveVectorA);
        colliderB->AddVelocity(resolveVectorB);

        colliderA->RemoveCollision(colliderB);
        colliderB->RemoveCollision(colliderA);
    }

    void PhysicsManager::ResolveDynamicVsStatic(RigidBody* colliderA, RigidBody* colliderB)
    {
        // TODO
        Vector3 distance = (colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition());

        float overlap = colliderA->GetRigidBodyRadius() + colliderB->GetRigidBodyRadius() - distance.GetMagnitude();

        colliderA->GetOwnerObject()->GetTransform()->AddPosition(distance.Normalized() * -overlap);

        distance = colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition();

        Vector3 velocityA = colliderA->GetVelocity();

        Vector3 resolveVectorA = distance * -2 * (velocityA).Dot(distance) / Square(distance.GetMagnitude());

        colliderA->AddVelocity(resolveVectorA);

        colliderA->RemoveCollision(colliderB);
        colliderB->RemoveCollision(colliderA);
    }
}
