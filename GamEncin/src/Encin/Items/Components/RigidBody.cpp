#include "Encin/Encin.h"

namespace GamEncin
{
    RigidBody::RigidBody(Object *obj) : Component(obj)
    {
        if (Application::IsRunning())
        {
            PhysicsManager::AddRigidBody(this);
        }
    }

    RigidBody::~RigidBody()
    {
        if (Application::IsRunning())
        {
            PhysicsManager::RemoveRigidBody(this);
        }
    }

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
        this->dragMultiplier = Clamp(drag, 0.0f, 1.0f);
    }

    void RigidBody::SetAngularDrag(float angularDrag)
    {
        this->angularDragMultiplier = Clamp(angularDrag, 0.0f, 1.0f);
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
        return dragMultiplier;
    }

    float RigidBody::GetAngularDrag()
    {
        return angularDragMultiplier;
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
        return linearVelocity;
    }

    Vector3 RigidBody::GetAngularVelocity()
    {
        return angularVelocity;
    }

    Vector3 RigidBody::GetAcceleration()
    {
        return linearAcceleration;
    }

    Vector3 RigidBody::GetAngularAcceleration()
    {
        return angularAcceleration;
    }

    vector<RigidBody *> RigidBody::GetCollisions()
    {
        return collisions;
    }

    void RigidBody::AddForce(Vector3 force)
    {
        linearAcceleration += force / mass;
    }

    void RigidBody::AddTorque(Vector3 torque)
    {
        angularAcceleration += torque / (mass * Square(colliderRadius) * 0.4f);
    }

    void RigidBody::AddLinearVelocity(Vector3 linearVelocity)
    {
        this->linearVelocity += linearVelocity;
    }

    void RigidBody::AddAngularVelocity(Vector3 angularVelocity)
    {
        this->angularVelocity += angularVelocity;
    }

    void RigidBody::ClearCollisions()
    {
        for (RigidBody *collider : collisions)
        {
            RemoveCollision(collider);
        }

        collisions.clear();
    }

    void RigidBody::AddCollision(RigidBody *body)
    {
        if (!body)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to add is null.");
            return;
        }

        auto obj = std::find(collisions.begin(), collisions.end(), body);

        if (obj != collisions.end())
        {
            // Application::PrintLog(ElementDuplicationErr, "RigidBody already exists in collisions.");
            return;
        }

        collisions.push_back(body);

        if (isTrigger)
        {
            object->OnTriggerEnter(body);
        }
        else
        {
            object->OnCollisionEnter(body);
        }
    }

    void RigidBody::RemoveCollision(RigidBody *body)
    {
        if (!body)
        {
            Application::PrintLog(NullPointerErr, "RigidBody trying to Remove is null.");
            return;
        }

        auto obj = std::find(collisions.begin(), collisions.end(), body);

        if (obj == collisions.end())
        {
            // Application::PrintLog(ElementCouldNotFindErr, "RigidBody couldn't find in collisions.");
            return;
        }

        if (isTrigger)
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
        for (RigidBody *collider : collisions)
        {
            if (!collider)
                continue;

            if (isTrigger)
            {
                object->OnTriggerStay(collider);
            }
            else
            {
                object->OnCollisionStay(collider);
            }
        }

        if (!isDynamic)
        {
            return;
        }

        // --- Linear ---
        Vector3 netForce = Vector3::Zero();

        // gravity
        Vector3 gravityForce = Vector3(0, -GRAVITY, 0) * gravityScale * mass;
        netForce += gravityForce;

        // linear drag
        Vector3 dragForce = linearVelocity * -dragMultiplier;
        netForce += dragForce;

        // custom linear acceleration
        if (linearAcceleration.GetMagnitude() > MIN_RB_LINEAR_ACCELERATION)
        {
            netForce += linearAcceleration * mass;
        }
        linearAcceleration = Vector3::Zero();

        // apply net force to linear velocity and clamp it
        linearVelocity += netForce / mass * Application::GetFixedDeltaTime();
        if (linearVelocity.GetMagnitude() < MIN_RB_LINEAR_VELOCITY)
        {
            linearVelocity = Vector3::Zero();
        }
        else if (linearVelocity.GetMagnitude() > MAX_RB_LINEAR_VELOCITY)
        {
            linearVelocity = linearVelocity.Normalized() * MAX_RB_LINEAR_VELOCITY;
        }

        // apply linear velocity to position
        object->GetTransform()->AddPosition(linearVelocity * Application::GetFixedDeltaTime());

        // --- Angular ---
        Vector3 netTorque = Vector3::Zero();

        // moment of inertia
        float momentOfInertia = 0.4f * mass * Square(colliderRadius);
        momentOfInertia = Clamp(momentOfInertia, MIN_RB_ANGULAR_VELOCITY, MAX_RB_ANGULAR_VELOCITY);

        // angular drag
        Vector3 angularDragForce = Square(angularVelocity) * -angularDragMultiplier;
        netTorque += angularDragForce;

        // custom angular acceleration
        if (angularAcceleration.GetMagnitude() > MIN_RB_ANGULAR_ACCELERATION)
        {
            netTorque += angularAcceleration * momentOfInertia;
        }
        angularAcceleration = Vector3::Zero();

        // apply net torque to angular velocity and clamp it
        angularVelocity += netTorque / momentOfInertia * Application::GetFixedDeltaTime();
        if (angularVelocity.GetMagnitude() < MIN_RB_ANGULAR_VELOCITY)
        {
            angularVelocity = Vector3::Zero();
        }
        else if (angularVelocity.GetMagnitude() > MAX_RB_ANGULAR_VELOCITY)
        {
            angularVelocity = angularVelocity.Normalized() * MAX_RB_ANGULAR_VELOCITY;
        }

        // apply angular velocity to rotation
        object->GetTransform()->AddRotation(angularVelocity * Application::GetFixedDeltaTime());
    }
}
