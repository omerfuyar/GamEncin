#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene = app.CreateAndUseScene();

    int row = 101;
    int col = 101;
    float gap = 3.0;

    Sphere* shape = scene.CreateObject<Sphere>();

    //for(int i = 0; i < col; i++)
    //{
    //    for(int j = 0; j < row; j++)
    //    {
    //        //Sphere* shape = scene.CreateObject<Sphere>();
    //        //Cube* shape = scene.CreateObject<Cube>();
    //        //Pyramid* shape = scene.CreateObject<Pyramid>();
    //        shape->position = Vector3((i - row / 2) * gap, (j - col / 2) * gap, 0);
    //        shape->rotation = Vector3((i + j) * gap / 10, (i + j) * gap / 10, (i + j) * gap / 10);
    //    }
    //}
}

void SetVariables(Application& app)
{
    app.fixedFPS = 50;
    app.windowSize = Vector2(1080, 1080);
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
