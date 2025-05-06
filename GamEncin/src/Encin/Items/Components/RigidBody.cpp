#include "Encin/Encin.h"

namespace GamEncin
{
    RigidBody::RigidBody(Object* obj) : Component(obj) {}

    void RigidBody::SetDynamic(bool isDynamic)
    {
        this->isDynamic = isDynamic;
    }

    void RigidBody::SetTrigger(bool isTrigger)
    {
        this->isTrigger = isTrigger;
    }

    void RigidBody::SetMass(float mass)
    {
        this->mass = Max(mass, MIN_RIGIDBODY_MASS);
    }

    void RigidBody::SetDrag(float drag)
    {
        this->drag = Clamp(drag, 0.0f, 1.0f);
    }

    void RigidBody::SetAngularDrag(float angularDrag)
    {
        this->angularDrag = Clamp(angularDrag, 0.0f, 1.0f);
    }

    void RigidBody::SetGravityScale(float gravityScale)
    {
        this->gravityScale = gravityScale;
    }

    void RigidBody::SetColliderRadius(float bodyRadius)
    {
        this->colliderRadius = Max(bodyRadius, 0);
    }

    bool RigidBody::IsDynamic()
    {
        return isDynamic;
    }

    bool RigidBody::IsTrigger()
    {
        return isTrigger;
    }

    float RigidBody::GetMass()
    {
        return mass;
    }

    float RigidBody::GetDrag()
    {
        return drag;
    }

    float RigidBody::GetAngularDrag()
    {
        return angularDrag;
    }

    float RigidBody::GetGravityScale()
    {
        return gravityScale;
    }

    float RigidBody::GetRigidBodyRadius()
    {
        return colliderRadius;
    }

    Vector3 RigidBody::GetVelocity()
    {
        return velocity;
    }

    Vector3 RigidBody::GetAngularVelocity()
    {
        return angularVelocity;
    }

    Vector3 RigidBody::GetAcceleration()
    {
        return acceleration;
    }

    Vector3 RigidBody::GetAngularAcceleration()
    {
        return angularAcceleration;
    }

    vector<RigidBody*> RigidBody::GetCollisions()
    {
        return collisions;
    }

    void RigidBody::AddForce(Vector3 force)
    {
        acceleration += force / mass;
    }

    void RigidBody::AddTorque(Vector3 torque)
    {
        angularAcceleration += torque / (mass * Square(colliderRadius) * 0.4f);
    }

    void RigidBody::AddVelocity(Vector3 velocity)
    {
        this->velocity += velocity;
    }

    void RigidBody::AddAngularVelocity(Vector3 angularVelocity)
    {
        this->angularVelocity += angularVelocity;
    }

    void RigidBody::ClearCollisions()
    {
        for(RigidBody* collider : collisions)
        {
            RemoveCollision(collider);
        }

        collisions.clear();
    }

    void RigidBody::AddCollision(RigidBody* body)
    {
        if(!body)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to add is null.");
            return;
        }

        auto obj = std::find(collisions.begin(), collisions.end(), body);

        if(obj != collisions.end())
        {
            Application::PrintLog(ElementDuplicationErr, "RigidBody already exists in collisions.");
            return;
        }

        collisions.push_back(body);

        if(isTrigger)
        {
            object->OnTriggerEnter(body);
        }
        else
        {
            object->OnCollisionEnter(body);
        }
    }

    void RigidBody::RemoveCollision(RigidBody* body)
    {
        if(!body)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to Remove is null.");
            return;
        }

        auto obj = std::find(collisions.begin(), collisions.end(), body);

        if(obj == collisions.end())
        {
            Application::PrintLog(ElementCouldNotFindErr, "RigidBody couldn't find in collisions.");
            return;
        }

        if(isTrigger)
        {
            object->OnTriggerExit(body);
        }
        else
        {
            object->OnCollisionExit(body);
        }

        collisions.erase(obj);
    }

    void RigidBody::FixUpdate()
    {
        for(RigidBody* collider : collisions)
        {
            if(!collider) continue;

            if(isTrigger)
            {
                object->OnTriggerStay(collider);
            }
            else
            {
                object->OnCollisionStay(collider);
            }
        }

        if(!isDynamic)
        {
            return;
        }

        //gravity and drag acceleration
        //acceleration -= Vector3(0, GRAVITY, 0) * gravityScale + velocity.Normalized() * Square(velocity.GetMagnitude()) * drag / mass;
        acceleration -= velocity * drag / mass;

        Vector3 gravity = Vector3(0, -GRAVITY, 0) * gravityScale;

        velocity += (acceleration + gravity) * Application::GetFixedDeltaTime();
        object->GetTransform()->AddPosition(velocity * Application::GetFixedDeltaTime());

        //angular drag acceleration
        //angularAcceleration -= angularVelocity.Normalized() * Square(angularVelocity.GetMagnitude()) * angularDrag / (0.4f * mass * Square(colliderRadius));
        angularAcceleration -= angularVelocity * angularDrag / (0.4f * mass * Square(colliderRadius));

        angularVelocity += angularAcceleration * Application::GetFixedDeltaTime();
        object->GetTransform()->AddRotation(angularVelocity * Application::GetFixedDeltaTime());

        //printf("\nFixUpdate RigidBody, %s\n", object->GetName().c_str());
        //printf("velocity: %f, %f, %f\n", velocity.x, velocity.y, velocity.z);
        //printf("angularVelocity: %f, %f, %f\n", angularVelocity.x, angularVelocity.y, angularVelocity.z);
        //printf("acceleration: %f, %f, %f\n", acceleration.x, acceleration.y, acceleration.z);
        //printf("angularAcceleration: %f, %f, %f\n", angularAcceleration.x, angularAcceleration.y, angularAcceleration.z);
        //printf("drag: %f\n", drag);
        //printf("angularDrag: %f\n", angularDrag);
        //printf("gravityScale: %f\n", gravityScale);
        //printf("mass: %f\n", mass);
        //printf("pos : %f %f %f\n", object->GetTransform()->GetGlobalPosition().x, object->GetTransform()->GetGlobalPosition().y, object->GetTransform()->GetGlobalPosition().z);
    }
}
