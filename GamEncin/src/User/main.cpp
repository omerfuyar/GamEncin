#include "GamEncin.h"
#include "UserObjects.h"

// Manually create and add objects to scene. If you want to make a new game object class, you can create a new class that inherits from Object class. This can be done with creating a new class in UserObjects.h.
void SceneBuilding()
{
	EntityManager entityManager;
	//SystemManager systemManager;

	cout << entityManager.CreateEntity();
	cout << entityManager.CreateEntity();
	cout << entityManager.CreateEntity();
	
}

int main(void)
{
	SceneBuilding();
	return 0;
}