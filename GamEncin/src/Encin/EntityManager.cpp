#include "ECS.h"

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
                Application::GetInstance().systemManager.End(-3);
                break;
        }
    }

    Component Entity::AddComponent(ComponentType componentT)
    {
        if(components.find(componentT) != components.end())
            Application::GetInstance().systemManager.End(-4); //Component already exists

        Component component = Component();

        switch(componentT)
        {
            case TransformCT:
                component = Transform();
                Application::GetInstance().systemManager.transformManager.transformComponents.insert({id, static_cast<Transform&>(component)});
                break;

            case PsychicsBodyCT:
                component = PsychicsBody();
                Application::GetInstance().systemManager.psychicsBodyManager.psychicsBodyComponents.insert({id, static_cast<PsychicsBody&>(component)});
                break;

            case RendererCT:
                component = Renderer();
                Application::GetInstance().systemManager.rendererManager.rendererComponents.insert({id, static_cast<Renderer&>(component)});
                break;

            default:
                Application::GetInstance().systemManager.End(-3); //Component type not found
                break;
        }

        components.insert({component.type, component});
        return component;
    }


    template <typename Type>
    Type Entity::GetComponent()
    {
        Type createdComponent = Type();
        if(components.find(createdComponent.type) != components.end())
        {
            return static_cast<Type&>(components.at(createdComponent.type));
        }
        else
        {
            Application::GetInstance().systemManager.End(-3);
        }
    }

    // Explicit instantiations
    template Transform Entity::GetComponent<Transform>();
    template PsychicsBody Entity::GetComponent<PsychicsBody>();
    template Renderer Entity::GetComponent<Renderer>();

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
