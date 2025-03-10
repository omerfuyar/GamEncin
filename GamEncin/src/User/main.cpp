#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene = app.CreateAndUseScene();

    int totalVerticeCount = 0, totalIndiceCount = 0, totalTriangleCount = 0;

    //myObject* shape = new myObject;
    //scene.AddObject(shape);
    //shape->scale = Vector3(0.2, 0.8, 0.2);

    vector<Object*> shapes =
    {
        new Cube(),
        new Sphere(),
        new Circle(),
        new Plane(),
        new Simit(),
        new Cone(),
        new Cylinder(),
        new Pyramid(),
    };

    int gridRows = 3;
    int gridCols = 3;
    float gap = 500.0;
    for(int i = 0; i < shapes.size(); i++)
    {
        scene.AddObject(shapes[i]);
        shapes[i]->scale = Vector3(0.25, 0.25, 0.25);

        int row = i / gridCols;
        int col = i % gridCols;
        shapes[i]->position = Vector3((col - gridCols / 2), (row - gridRows / 2), 0) * gap;

        totalIndiceCount += shapes[i]->indices.size() / 2;
        totalVerticeCount += shapes[i]->vertices.size();
        totalTriangleCount += shapes[i]->indices.size() / 3;
    }

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
