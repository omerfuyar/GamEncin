#pragma once

namespace GamEncin
{
    class Object;

    class Component
    {
    public:
        Component() = default;
        Component(Object* obj);
        ~Component();

        //Get the object that owns this component
        Object* GetOwnerObject() const;

        //Set the object that owns this component
        void SetObject(Object* obj);

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
