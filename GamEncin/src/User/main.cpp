#include "GamEncin.h"
#include "UserObjects.h"

// Manually create and add objects to scene. If you want to make a new game object class, you can create a new class that inherits from Object class. This can be done with creating a new class in UserObjects.h.
static void SceneBuilding()
{
	auto object1 = std::make_shared<TestObject>();
	SceneManager::GetInstance().AddObject(object1);
}

int main(void)
{
	SceneBuilding();

	SceneManager::GetInstance().Render();

	SceneManager::GetInstance().GameLoops();

	return 0;
}