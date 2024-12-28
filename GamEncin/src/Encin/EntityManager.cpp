#include "ECS.h"
namespace GamEncin
{
	Entity::Entity()
	{
		id = 0;
		tag = "Default Tag";
		layer = Default;
		name = "Object";
		components = vector<Component>();
	}

	void Entity::AddComponent(Component component)
	{

	}

	void Entity::RemoveComponent(Component component)
	{

	}

	Component* Entity::GetComponent()
	{
		return nullptr;
	}

#pragma region Entity Manager

	EntityID EntityManager::CreateEntity()
	{
		last_id++;
		entities_components.insert({last_id, Entity()});
		return last_id;
	}

	void EntityManager::DestroyEntity(Entity* entity)
	{

	}
}