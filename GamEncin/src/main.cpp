#include "AnyObject.h"
#include "GamEncin.h"

// Manually create and add objects to scene
static void SceneBuilding()
{
	auto object1 = std::make_shared<AnyObject>();
	SceneManager::GetInstance().AddObject(object1);

}

int main(void)
{
	SceneBuilding();

	SceneManager::GetInstance().GameLoops();

	return 0;
}