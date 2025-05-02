#pragma once
#include "Encin/Items/Components/Component.h"

#define MIN_RIGIDBODY_MASS 0.001f

namespace GamEncin
{
    /// <summary>
    /// RigidBody component for physics. it is integrated with a sphere collider for now.
    /// </summary>
    class RigidBody : public Component
    {
    public:
        RigidBody(Object* obj);

        // Set the body to be dynamic or static
        void SetDynamic(bool isDynamic);
        // Set the body to be a trigger or not
        void SetTrigger(bool isTrigger);
        // Set body's mass, min mass is MIN_RIGIDBODY_MASS macro
        void SetMass(float mass);
        // Set body's drag between 0 and 1, 1 is full drag
        void SetDrag(float drag);
        // Set body's angular drag between 0 and 1, 1 is full drag
        void SetAngularDrag(float angularDrag);
        // Set body's gravity scale, 1 is normal gravity
        void SetGravityScale(float gravityScale);
        // Set body's collider radius, min radius is 0
        void SetColliderRadius(float colliderRadius);

        bool IsDynamic();
        bool IsTrigger();
        float GetMass();
        float GetDrag();
        float GetAngularDrag();
        float GetGravityScale();
        float GetRigidBodyRadius();
        Vector3 GetVelocity();
        Vector3 GetAngularVelocity();
        Vector3 GetAcceleration();
        Vector3 GetAngularAcceleration();
        vector<RigidBody*> GetCollisions();

        // shouldn't be used by user
        void AddCollision(RigidBody* collider);
        // shouldn't be used by user
        void RemoveCollision(RigidBody* collider);
        // Add force to the body. Relative to it's mass
        void AddForce(Vector3 force);
        // Add torque to the body. Relative to it's moment of inertia
        void AddTorque(Vector3 torque);
        // Add velocity to the body. 
        void AddVelocity(Vector3 velocity);
        // Add angular velocity to the body.
        void AddAngularVelocity(Vector3 angularVelocity);

    private:
        bool isDynamic = true;
        bool isTrigger = false;

        float mass = 1.0f;
        float drag = 0.0f;
        float angularDrag = 0.0f;
        float gravityScale = 1.0f;

        float colliderRadius = 1.0f;

        Vector3 velocity = Vector3::Zero();
        Vector3 angularVelocity = Vector3::Zero();
        Vector3 acceleration = Vector3::Zero();
        Vector3 angularAcceleration = Vector3::Zero();

        vector<RigidBody*> collisions;

        void FixUpdate() override;
    };
}
