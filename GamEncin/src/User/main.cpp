#include "GamEncin.h"
#include "UserObjects.h"

void SceneBuilding()
{
    Scene scene;

    Object& object1 = scene.CreateObject();
}

int main(void)
{
    SceneBuilding();

    Application& app = Application::GetInstance();

    app.Run();

    return 0;
}
