#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/OpenGLObjects.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Object;
    class RigidBody;

    class Component
    {
    public:
        Component() = default;
        Component(Object* obj);
        ~Component();

        //Get the object that owns this component
        Object* const GetOwnerObject();

        //Set the object that owns this component
        void SetObject(Object* obj);

        virtual void OnTriggerEnter(RigidBody* enteredRigidBody) {};
        virtual void OnTriggerStay(RigidBody* stayingRigidBody) {};
        virtual void OnTriggerExit(RigidBody* exitedRigidBody) {};

        virtual void OnCollisionEnter(RigidBody* enteredRigidBody) {};
        virtual void OnCollisionStay(RigidBody* stayingRigidBody) {};
        virtual void OnCollisionExit(RigidBody* exitedRigidBody) {};

        virtual void OnEnable() {}
        virtual void OnDisable() {}

        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void FixUpdate() {}
        virtual void StartOfSecond() {}

    protected:
        Object* object = nullptr;
    };
}
