#include "GamEncin.h"
#include "UserObjects.h"

// Manually create and add objects to scene. If you want to make a new game object class, you can create a new class that inherits from Object class. This can be done with creating a new class in UserObjects.h.
void SceneBuilding()
{
    Application& app = Application::GetInstance();
    EntityManager& entityManager = EntityManager::GetInstance();

    //Entity qwe = entityManager.CreateEntity();
    //
    //Transform zxc = qwe.AddComponent<Transform>();
    //Transform hmhg = qwe.GetComponent<Transform>();
    //
    //if(hmhg == zxc)
    //    cout << "true" << "\n";
    //else
    //    cout << "false" << "\n";
    //
    //cout << "hmhg: " << hmhg.position.x << "\n";
    //cout << "zxc: " << zxc.position.x << "\n";
}

int main(void)
{
    SceneBuilding();
    return 0;
}
