#include "ECS.h"
#include <memory>

namespace GamEncin
{

#pragma region Entity

    template <typename Type>
    void Entity::RemoveComponent()
    {
        Type tempComponent;
        ComponentType componentType = tempComponent.type;
        SystemManager& systemManager = Application::GetInstance().systemManager;

        if(components.find(componentType) == components.end())
        {
            Application::GetInstance().End(-12); //Component not found in entity
        }

        components.erase(componentType);

        switch(componentType)
        {
            case TransformCT:
                systemManager.transformManager.transformComponents.erase(id);
                break;
            case PsychicsBodyCT:
                systemManager.psychicsBodyManager.psychicsBodyComponents.erase(id);
                break;
            case RendererCT:
                systemManager.rendererManager.rendererComponents.erase(id);
                break;
            default:
                Application::GetInstance().End(-13); //Component type not found
                break;
        }
    }

    template <typename Type>
    Type& Entity::AddComponent()
    {
        Type* createdComponent = new Type();
        ComponentType componentType = createdComponent->type;
        SystemManager& systemManager = Application::GetInstance().systemManager;

        if(components.find(componentType) != components.end())
        {
            Application::GetInstance().End(-14); //Component already exists
        }

        components[componentType] = createdComponent;

        switch(componentType)
        {
            case TransformCT:
                systemManager.transformManager.transformComponents[id] = dynamic_cast<Transform*>(components[componentType]);
                break;
            case PsychicsBodyCT:
                systemManager.psychicsBodyManager.psychicsBodyComponents[id] = dynamic_cast<PsychicsBody*>(components[componentType]);
                break;
            case RendererCT:
                systemManager.rendererManager.rendererComponents[id] = dynamic_cast<Renderer*>(components[componentType]);
                break;
            default:
                Application::GetInstance().End(-13); //Component type not found
                break;
        }

        return *static_cast<Type*>(components[componentType]);
    }

    template <typename Type>
    Type& Entity::GetComponent()
    {
        Type tempComponent;
        ComponentType componentType = tempComponent.type;

        if(components.find(componentType) == components.end())
        {
            Application::GetInstance().End(-12); //Component not found in entity
        }

        return *static_cast<Type*>(components[componentType]);
    }

    //Explicit template instantiation
    template Transform& Entity::AddComponent<Transform>();
    template PsychicsBody& Entity::AddComponent<PsychicsBody>();
    template Renderer& Entity::AddComponent<Renderer>();

    template void Entity::RemoveComponent<Transform>();
    template void Entity::RemoveComponent<PsychicsBody>();
    template void Entity::RemoveComponent<Renderer>();

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
