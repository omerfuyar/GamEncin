#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene = app.CreateAndUseScene();

    int totalVerticeCount = 0, totalIndiceCount = 0, totalTriangleCount = 0;

    myObject* shape = new myObject;
    scene.AddObject(shape);
    //shape->scale = Vector3(0.2, 0.8, 0.2);

    totalIndiceCount += shape->indices.size() / 2;
    totalVerticeCount += shape->vertices.size();
    totalTriangleCount += shape->indices.size() / 3;

    int row = 101;
    int col = 101;
    float gap = 15.0;
    //for(int i = 0; i < col; i++)
    //{
    //    for(int j = 0; j < row; j++)
    //    {
    //        Sphere* shape = new Sphere(1.0, 20, 20);
    //        scene.AddObject(shape);
    //        shape->scale /= 100;
    //        totalIndiceCount += shape->indices.size();
    //        totalVerticeCount += shape->vertices.size();
    //        totalTriangleCount += shape->indices.size() / 3;
    //        //Cube* shape = scene.CreateObject<Cube>();
    //        //Pyramid* shape = scene.CreateObject<Pyramid>();
    //        shape->position = Vector3((i - row / 2), (j - col / 2), 0) * gap;
    //        shape->rotation = Vector3((i + j), (i + j), (i + j)) * gap / 10;
    //    }
    //}

    printf("Sphere vertice count: %d\n", totalVerticeCount);
    printf("Sphere indice count: %d\n", totalIndiceCount);
    printf("Sphere triangle count: %d\n", totalTriangleCount);
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
