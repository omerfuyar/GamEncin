#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene = app.CreateAndUseScene();

    TestObject& object1 = scene.CreateObject<TestObject>();
    TestObject& object2 = scene.CreateObject<TestObject>();

    object1.vertices = {
        Vector3(0.52f, 0.5f, 0.0f),
        Vector3(0.52f, -0.5f, 0.0f),
        Vector3(-0.48f, -0.5f, 0.0f)
    };

    object1.indices = {
        0, 1, 2
    };

    object2.vertices = {
        Vector3(0.5f, 0.5f, 0.0f),
        Vector3(-0.5f, 0.5f, 0.0f),
        Vector3(-0.5f, -0.5f, 0.0f)
    };

    object2.indices = {
        0, 1, 2
    };
}

int main(void)
{
    Application& app = Application::GetInstance();

    SceneBuilding(app);

    app.Run();

    return 0;
}
