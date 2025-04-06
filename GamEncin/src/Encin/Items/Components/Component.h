#pragma once

namespace GamEncin
{
    class Object;

    class Component
    {
    public:
        Component(Object* object);
        ~Component();

        Object* object = nullptr;

        virtual void Awake() {}
        virtual void Start() {}
        virtual void Update() {}
        virtual void LateUpdate() {}
        virtual void FixUpdate() {}
        virtual void StartOfSecond() {}
    };
}
