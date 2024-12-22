#pragma once
#include <iostream>
#include <memory>
#include <vector>
using std::vector;

namespace GamEncin
{
	//Objects Loop Manager
	class SceneManager
	{
	private:
		vector<std::shared_ptr<Object>> objects;
	public:
		void AddObject(std::shared_ptr<Object> obj)
		{
			objects.push_back(obj);
		}

		void Awake()
		{
			for(auto& obj : objects)
				obj->Awake();
		}
		
		void Start()
		{
			for(auto& obj : objects)
				obj->Start();
		}

		void Update()
		{
			for(auto& obj : objects)
				obj->Update();
		}

		void FixUpdate()
		{
			for(auto& obj : objects)
				obj->FixUpdate();
		}
	};
}