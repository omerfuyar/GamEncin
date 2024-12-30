#include "ECS.h"
#include <memory>

namespace GamEncin
{

#pragma region Entity

    Entity::Entity(EntityID createdID)
    {
        id = createdID;
    }

    void Entity::RemoveComponent(ComponentType componentT)
    {
        components.erase(componentT);
        switch(componentT)
        {
            case TransformCT:
                Application::GetInstance().systemManager.transformManager.transformComponents.erase(id);
                break;
            case PsychicsBodyCT:
                Application::GetInstance().systemManager.psychicsBodyManager.psychicsBodyComponents.erase(id);
                break;
            case RendererCT:
                Application::GetInstance().systemManager.rendererManager.rendererComponents.erase(id);
                break;
            default:
                Application::GetInstance().systemManager.End(-13); //Component type not found
                break;
        }
    }

    template <typename Type>
    Type& Entity::AddComponent()
    {
        auto createdComponent = std::make_unique<Type>();
        ComponentType componentType = createdComponent->type;

        SystemManager& systemManager = Application::GetInstance().systemManager;

        if(components.find(componentType) != components.end())
            systemManager.End(-14); //Component already exists

        switch(componentType)
        {
            case TransformCT:
                systemManager.transformManager.transformComponents[id] = dynamic_cast<Transform*>(createdComponent);
                break;
            case PsychicsBodyCT:
                systemManager.psychicsBodyManager.psychicsBodyComponents[id] = dynamic_cast<PsychicsBody*>(createdComponent);
                break;
            case RendererCT:
                systemManager.rendererManager.rendererComponents[id] = dynamic_cast<Renderer*>(createdComponent);
                break;
            default:
                systemManager.End(-13); //Component type not found
                break;
        }

        //components[componentType] = dynamic_cast<Type&>(createdComponent);
        components[componentType] = std::move(createdComponent);

        return *dynamic_cast<Type*>(components[componentType]);
    }

    template <typename Type>
    Type& Entity::GetComponent()
    {
        Type createdComponent;
        ComponentType componentType = createdComponent.type;

        if(components.find(componentType) != components.end())
        {
            return static_cast<Type&>(components[componentType]);
        }
        else
        {
            Application::GetInstance().systemManager.End(-12); //Component not found in entity
        }
    }

    // Explicit instantiations
    template Transform& Entity::AddComponent<Transform>();
    template PsychicsBody& Entity::AddComponent<PsychicsBody>();
    template Renderer& Entity::AddComponent<Renderer>();

    template Transform& Entity::GetComponent<Transform>();
    template PsychicsBody& Entity::GetComponent<PsychicsBody>();
    template Renderer& Entity::GetComponent<Renderer>();

#pragma endregion

#pragma region Entity Manager

    Entity EntityManager::CreateEntity()
    {
        last_id++;
        Entity createdEntity = Entity(last_id);
        entities.insert({last_id, createdEntity});
        return createdEntity;
    }

    void EntityManager::DestroyEntity(Entity entity)
    {
        entities.erase(entity.id);
    }

#pragma endregion
}
