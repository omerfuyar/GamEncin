#pragma once
#include "Tools.h"

namespace GamEncin
{
#pragma region Entity

    using Entity = unsigned int;
    constexpr int MaxEntities = 1000;

    class EntityManager
    {
        Entity CreateEntity();
        void DestroyEntity(Entity entity);
    };

#pragma endregion

#pragma region Components

    struct DefaultObject
    {
        bool active = true;
        string
            name = "Object",
            tag = "Default Tag";
        Layer layer = Default;
    };

    struct CTransform
    {
    public:
        Vector3
            position,
            rotation,
            scale = Vector3::One();
    };

    struct CPsychicsBody
    {
    public:
        Vector3
            velocity,
            acceleration,
            force;

        float
            mass = 1,
            drag = 0,
            angularDrag = 0,
            gravityScale = 1;
    };

    struct CRenderer
    {
    public:
        vector<Vector3> vertices;
    };

#pragma endregion

#pragma region Systems

    array<DefaultObject, MaxEntities> objects;
    array<CTransform, MaxEntities> transforms;
    array<CPsychicsBody, MaxEntities> bodies;
    array<CRenderer, MaxEntities> renderers;

    void UpdatePsyhics();
    void UpdateRenderers();

#pragma endregion
}
