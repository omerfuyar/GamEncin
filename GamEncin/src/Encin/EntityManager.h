#pragma once
#include "Tools.h"

namespace GamEncin
{
	class Entity; // Forward declaration

	using EntityID = unsigned int;
	map<EntityID, Component> entities_components;

	class Entity : public Component
	{
	public:
		EntityID id;
		string tag;
		Layer layer;
		string name;
		vector<Component> components;

		Entity();
		~Entity() = default;

		void AddComponent(Component component);
		void RemoveComponent(Component component);
		Component* GetComponent();
	};

	class EntityManager
	{
	public:
		EntityID last_id = 0;

		EntityManager() = default;
		~EntityManager() = default;

		EntityID CreateEntity();
		void DestroyEntity(Entity* entity);
	};
}