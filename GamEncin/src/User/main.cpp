#include "GamEncin.h"
#include "UserObjects.h"

// Manually create and add objects to scene. If you want to make a new game object class, you can create a new class that inherits from Object class. This can be done with creating a new class in UserObjects.h.
void SceneBuilding()
{
	auto object1 = std::make_shared<TestObject>();
	SceneManager::GetInstance().AddObject(object1);
}

int main(void)
{
	SceneBuilding();

	// Create window and initialize OpenGL before everything
	SceneManager::GetInstance().InitialRender();

	// Start the game loop
	SceneManager::GetInstance().GameLoops();

	return 0;
}