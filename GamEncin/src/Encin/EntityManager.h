#pragma once
#include "Tools.h"

namespace GamEncin
{
    using EntityID = unsigned int;

    class Entity
    {
    public:
        EntityID id;
        unordered_map<ComponentType, Component> components;

        Entity(EntityID createdID);
        ~Entity() = default;

        void AddComponent(Component component);
        void RemoveComponent(Component component);
        Component GetComponent(Component component);
    };

    class EntityManager
    {
    public:
        EntityID last_id = 0;

        EntityManager() = default;
        ~EntityManager() = default;

        Entity CreateEntity();
        void DestroyEntity(Entity entity);
    };
}
