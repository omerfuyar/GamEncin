#pragma once
#include "Components.h"
#include <chrono>
#include <thread>
#include <vector>
using namespace std::chrono;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;

namespace GamEncin
{
	//Objects Loop Manager
	class SceneManager
	{
	private:
		SceneManager() {}

	public:
		static vector<shared_ptr<Object>> objects; //Objects in the scene
		static unique_ptr<SceneManager> INSTANCE; //Singleton Instance

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		static SceneManager& GetInstance()
		{
			if(!INSTANCE)
			{
				INSTANCE.reset(new SceneManager());
			}
			return *INSTANCE;
		}

		static void AddObject(shared_ptr<Object> obj)
		{
			objects.push_back(obj);
		}

		static void Awake()
		{
			for(auto& obj : objects)
				obj->Awake();
		}

		static void Start()
		{
			for(auto& obj : objects)
				obj->Start();
		}

		static void Update()
		{
			for(auto& obj : objects)
				obj->Update();
		}

		static void FixUpdate()
		{
			for(auto& obj : objects)
				obj->FixUpdate();
		}

		static void GameLoops()
		{
#pragma region definitions

			// Frame Rate Limit
			const int FPSlimit = 100;
			const auto frameDelay = 1000 / FPSlimit;
			bool haveFPSlimit = false;

			// Fixed Game loop
			const int fixFPSlimit = 50;
			const auto fixedDelay = 1000 / fixFPSlimit;

			auto lastFixedUpdate = high_resolution_clock::now();
			int frameCount = 0;
			int fps = 0;
			long long msPastFromStart = 0;

#pragma endregion

			// Awake all objects
			Awake();

			// Start all objects
			Start();

			while(true)
			{
				// Update all objects
				Update();

				auto now = high_resolution_clock::now();
				auto fixElapsed = duration_cast<milliseconds>(now - lastFixedUpdate);

				if(fixElapsed.count() >= fixedDelay)
				{
					FixUpdate();
					lastFixedUpdate = now;
					msPastFromStart += fixedDelay;

					if(msPastFromStart % 1000 == 0)
					{
						fps = frameCount;
						frameCount = 0;
						//std::cout << fps << "\n";
					}
				}

				// Simulate frame delay
				if(haveFPSlimit)
					std::this_thread::sleep_for(milliseconds(frameDelay));

				frameCount++;
			}
		}
	};
}