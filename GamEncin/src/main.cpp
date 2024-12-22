#include "AnyObject.h"
#include "GamEncin.h"
#include <chrono>
#include <iostream>
#include <thread>
using namespace std::chrono;

int main()
{
	SceneManager goManager;

	// Create and add objects
	auto object1 = make_shared<AnyObject>();
	goManager.AddObject(object1);

	// Call Start once before the loop
	goManager.Start();

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

	while(true)
	{
		// Update all objects
		goManager.Update();

		// FixedUpdate at a consistent interval
		auto now = high_resolution_clock::now();
		auto fixElapsed = duration_cast<milliseconds>(now - lastFixedUpdate);

		if(fixElapsed.count() >= fixedDelay)
		{
			goManager.FixUpdate();
			lastFixedUpdate = now;
			msPastFromStart += fixedDelay;

			if(msPastFromStart % 1000 == 0)
			{
				fps = frameCount;
				frameCount = 0;
				cout << fps << "\n";
			}
		}

		// Simulate frame delay
		if(haveFPSlimit)
			this_thread::sleep_for(milliseconds(frameDelay));

		frameCount++;
	}

	return 0;
}