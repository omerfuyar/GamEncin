#include "ECS.h"
#include <algorithm>

using std::find;
using std::distance;

namespace GamEncin
{
	unordered_map<EntityID, Entity> entities;

	Entity::Entity(EntityID createdID)
	{
		id = createdID;
	}

	void Entity::AddComponent(Component component)
	{	
		components.insert({component.type, component});
	}

	void Entity::RemoveComponent(Component component)
	{
		components.erase(find(components.begin(), components.end(), component));
	}


	Component Entity::GetComponent(Component component)
	{
		if(components.find(component.type) != components.end())
		{
			return components.at(component.type);
		}
		else
		{
			return NullComponent();
		}
	}

#pragma region Entity Manager

	Entity EntityManager::CreateEntity()
	{
		last_id++;
		Entity createdEntity = Entity(last_id);
		entities.insert({last_id, createdEntity});
		createdEntity.AddComponent(Object());
		return createdEntity;
	}

	void EntityManager::DestroyEntity(Entity entity)
	{
		entities.erase(entity.id);
	}

#pragma endregion
}