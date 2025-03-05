#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene = app.CreateAndUseScene();

    TestObject& object1 = scene.CreateObject<TestObject>();

    object1.modelVertices = {
        Vector3(-0.5, -0.5, -0.5),
        Vector3(0,0,0),
        Vector3(0.5, -0.5, -0.5),
        Vector3(255,0,0),
        Vector3(0.5, -0.5, 0.5),
        Vector3(0,255,0),
        Vector3(-0.5, -0.5, 0.5),
        Vector3(0,0,255),
        Vector3(-0.5, 0.5, 0.5),
        Vector3(255,255,255),
        Vector3(0.5, 0.5, 0.5),
        Vector3(255,255,0),
        Vector3(0.5, 0.5, -0.5),
        Vector3(0,255,255),
        Vector3(-0.5, 0.5, -0.5),
        Vector3(255,0,255)
    };

    object1.scale = Vector3(0.5, 1.6, 1);

    printf("%f, %f, %f", object1.position.x, object1.position.y, object1.position.z);

    object1.modelIndices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 6, 6, 7, 0,
        1, 2, 5, 5, 6, 1,
        2, 3, 4, 4, 5, 2,
        3, 0, 7, 7, 4, 3
    };
}

int main()
{
    Application& app = Application::GetInstance();

    SceneBuilding(app);
    app.Run();

    return 0;
}
