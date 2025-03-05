#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene = app.CreateAndUseScene();

    TestObject& object1 = scene.CreateObject<TestObject>();

    object1.vertices =
    {
        // Position                // Color
        Vector3(-0.5, -0.5, -0.5), Vector3(255, 0, 0),
        Vector3(0.5, -0.5, -0.5), Vector3(0, 255, 0),
        Vector3(0.5, -0.5, 0.5), Vector3(0, 0, 255),
        Vector3(-0.5, -0.5, 0.5), Vector3(255, 255, 0),
        Vector3(-0.5, 0.5, 0.5), Vector3(255, 0, 255),
        Vector3(0.5, 0.5, 0.5), Vector3(0, 255, 255),
        Vector3(0.5, 0.5, -0.5), Vector3(255, 255, 255),
        Vector3(-0.5, 0.5, -0.5), Vector3(0, 0, 0)
    };

    object1.scale = Vector3(0.5, 1.6, 1);

    object1.position = Vector3(-0.5, 0, 1);

    object1.indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 6, 6, 7, 0,
        1, 2, 5, 5, 6, 1,
        2, 3, 4, 4, 5, 2,
        3, 0, 7, 7, 4, 3};
}

void SetVariables(Application& app)
{
    app.fixedFPS = 50;
    app.windowSize = Vector2(720, 720);
    app.printFPS = true;
    app.renderer->clearColor = Vector4(0.2, 0.3, 0.3, 1.0);
    app.renderer->positionDivider = 1000;
}

int main()
{
    Application& app = *new Application();

    SceneBuilding(app);
    SetVariables(app);

    app.Run();

    return 0;
}
