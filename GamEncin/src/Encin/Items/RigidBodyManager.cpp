#include "Encin/Encin.h"

namespace GamEncin
{
    vector<RigidBody*> RigidBodyManager::colliders;

    void RigidBodyManager::AddRigidBody(RigidBody* collider)
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

    void RigidBodyManager::RemoveRigidBody(RigidBody* collider)
    {
        if(!collider)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to remove is null.");
            return;
        }

        auto obj = std::find(colliders.begin(), colliders.end(), collider);

        if(obj == colliders.end())
        {
            Application::PrintLog(ElementCouldNotFoundErr, "RigidBody couldn't find in the collider manager.");
            return;
        }

        colliders.erase(obj);
    }

    void RigidBodyManager::RemoveAllRigidBodies()
    {
        colliders.clear();
    }

    void RigidBodyManager::UpdateRigidBodies()
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
            }
        }
    }

    void RigidBodyManager::ResolveCollisions()
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

    bool RigidBodyManager::CheckForCollision(RigidBody* colliderA, RigidBody* colliderB)
    {
        if(!colliderA || !colliderB)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to check is null.");
            return false;
        }

        return (colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition()).GetMagnitude() < (colliderA->GetRigidBodyRadius() + colliderB->GetRigidBodyRadius());
    }

    void RigidBodyManager::ResolveDynamicVsDynamic(RigidBody* colliderA, RigidBody* colliderB)
    {
        Vector3 distance = (colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition());

        Vector3 velocityA = colliderA->GetVelocity();
        Vector3 velocityB = colliderB->GetVelocity();

        float massA = colliderA->GetMass();
        float massB = colliderB->GetMass();

        Vector3 resolveVectorA = (distance / Square(distance.GetMagnitude()) * (velocityB - velocityA).Dot(distance)) * (2 * massB / (massA + massB));
        Vector3 resolveVectorB = (distance / Square(distance.GetMagnitude()) * (velocityA - velocityB).Dot(distance)) * (2 * massA / (massA + massB));

        colliderA->AddVelocity(resolveVectorA);
        colliderB->AddVelocity(resolveVectorB);

        colliderA->RemoveCollision(colliderB);
        colliderB->RemoveCollision(colliderA);
    }

    void RigidBodyManager::ResolveDynamicVsStatic(RigidBody* colliderA, RigidBody* colliderB)
    {
        Vector3 distance = (colliderA->GetOwnerObject()->GetTransform()->GetGlobalPosition() - colliderB->GetOwnerObject()->GetTransform()->GetGlobalPosition());

        Vector3 velocityA = colliderA->GetVelocity();

        float massA = colliderA->GetMass();
        //TODO
        Vector3 resolveVectorA = distance * -2 * (velocityA).Dot(distance) / Square(distance.GetMagnitude());

        colliderA->AddVelocity(resolveVectorA);

        colliderA->RemoveCollision(colliderB);
        colliderB->RemoveCollision(colliderA);
    }
}
