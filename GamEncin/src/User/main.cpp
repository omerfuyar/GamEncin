#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene = app.CreateAndUseScene();

    TestObject& object1 = scene.CreateObject<TestObject>();

    object1.modelVertices = {
        Vector3(-0.5, -0.5, -0.5),
        Vector3(0.5, -0.5, -0.5),
        Vector3(0.5, -0.5, 0.5),
        Vector3(-0.5, -0.5, 0.5),
        Vector3(-0.5, 0.5, 0.5),
        Vector3(0.5, 0.5, 0.5),
        Vector3(0.5, 0.5, -0.5),
        Vector3(-0.5, 0.5, -0.5)
    };

    object1.indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 6, 6, 7, 0,
        1, 2, 5, 5, 6, 1,
        2, 3, 4, 4, 5, 2,
        3, 0, 7, 7, 4, 3
    };
}

void main()
{
    Application& app = Application::GetInstance();

    SceneBuilding(app);

    app.Run();
}
