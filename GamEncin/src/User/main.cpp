#include "GamEncin.h"
#include "UserObjects.h"

void SceneBuilding(Application& app)
{
    Scene& scene1 = app.CreateAndUseScene();

    Object& object1 = scene1.CreateObject();
}

int main(void)
{
    Application& app = Application::GetInstance();

    SceneBuilding(app);

    app.Run();

    return 0;
}
