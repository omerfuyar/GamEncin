#pragma once
#include "Tools.h"

namespace GamEncin
{
    using EntityID = unsigned int;

    class Entity
    {
    public:
        EntityID id;
        unordered_map<ComponentType, Component*> components;
        string name = "Object";
        string tag = "Default Tag";
        Layer layer = Default;

        Entity(EntityID createdID)
        {
            id = createdID;
        }
        ~Entity() = default;

        //Adds a new component to the entity and returns it
        template <class Type>
        Type& AddComponent();

        //Removes a specific component type
        template <class Type>
        void RemoveComponent();

        //Gets a specific component in the entity
        template <class Type>
        Type& GetComponent();
    };

    class EntityManager //singleton
    {
    private:
        EntityManager() = default;
        ~EntityManager() = default;

    public:
        map<EntityID, Entity> entities;

        EntityManager(const EntityManager&) = delete;
        void operator=(const EntityManager&) = delete;

        static EntityManager& GetInstance()
        {
            static EntityManager instance;
            return instance;
        }

        EntityID last_id = 0;

        //Creates a new entity instance
        Entity CreateEntity();

        //Destroys a spesisific entity
        void DestroyEntity(Entity entity);
    };
}
