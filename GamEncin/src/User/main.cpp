#include "GamEncin.h"
#include "UserObjects.h"

// Manually create and add objects to scene. If you want to make a new game object class, you can create a new class that inherits from Object class. This can be done with creating a new class in UserObjects.h.
void SceneBuilding()
{
    Application& app = Application::GetInstance();
    EntityManager& entityManager = EntityManager::GetInstance();

    Entity entity1 = entityManager.CreateEntity();

    cout << entity1.id << "\n";
    cout << &entity1.AddComponent<Transform>() << "\n";
    cout << &entity1.GetComponent<Transform>() << "\n";

    cout << &app.systemManager.transformManager.transformComponents[entity1.id] << "\n";
    cout << &app.systemManager.transformManager.transformComponents.at(entity1.id) << "\n";
}

int main(void)
{
    SceneBuilding();
    return 0;
}
