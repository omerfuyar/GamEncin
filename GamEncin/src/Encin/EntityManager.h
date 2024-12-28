#pragma once
#include "Tools.h"

namespace GamEncin
{
	class Entity;

	using EntityID = unsigned int;
	map<EntityID, Entity> entities;

	class Entity
	{
	public:
		EntityID id;
		string tag = "Default Tag";
		Layer layer = Default;
		string name = "Object";
		vector<Component> components;

		Entity() = default;
		~Entity() = default;

		void AddComponent();
		void RemoveComponent();
		void GetComponent();
	};

	class EntityManager
	{
	public:
		EntityManager() = default;
		~EntityManager() = default;

		void CreateEntity();
		void DestroyEntity();
	};
}