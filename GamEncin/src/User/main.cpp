#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene = app.CreateAndUseScene();

    int totalVerticeCount = 0, totalIndiceCount = 0, totalTriangleCount = 0;

    //vector<Object*> shapes =
    //{
    //    new Cube(),
    //    new Sphere(),
    //    new Circle(),
    //    new Plane(),
    //    new Simit(),
    //    new Cone(),
    //    new Cylinder(),
    //    new Pyramid(),
    //};

    int gridRows = 51;
    int gridCols = 51;
    float gap = 3.0;
    //for(int i = 0; i < shapes.size(); i++)
    //{
    //    scene.AddObject(shapes[i]);
    //    shapes[i]->scale = Vector3(0.25, 0.25, 0.25);
    //
    //    int row = i / gridCols;
    //    int col = i % gridCols;
    //    shapes[i]->position = Vector3((col - gridCols / 2), (row - gridRows / 2), 0) * gap;
    //
    //    totalIndiceCount += shapes[i]->indices.size();
    //    totalVerticeCount += shapes[i]->vertices.size();
    //    totalTriangleCount += shapes[i]->indices.size() / 3;
    //}

    //for(int i = 0; i < gridCols; i++)
    //{
    //    for(int j = 0; j < gridRows; j++)
    //    {
    //        myObject* shape = new myObject();
    //        scene.AddObject(shape);
    //
    //        shape->position = Vector3((i - gridRows / 2), (j - gridCols / 2), 0) * gap;
    //        shape->rotation = Vector3((i + j), (i + j), (i + j)) * gap / 4;
    //
    //        totalIndiceCount += shape->indices.size();
    //        totalVerticeCount += shape->vertices.size();
    //        totalTriangleCount += shape->indices.size() / 3;
    //    }
    //}

    myObject* shape = new myObject();
    scene.AddObject<myObject>(shape);
    shape->rotation = Vector3(-90, 0, 0);
    shape->position = Vector3(1.2, 1.2, -5);

    myObject* shape1 = new myObject();
    scene.AddObject<myObject>(shape1);
    shape1->rotation = Vector3(-90, 0, 0);
    shape1->position = Vector3(-0.5, -0.5, -2);

    myObject* shape2 = new myObject();
    scene.AddObject<myObject>(shape2);
    shape2->rotation = Vector3(-90, 0, 0);
    shape2->position = Vector3(0, 0, -10);

    //totalIndiceCount += shape->indices.size();
    //totalVerticeCount += shape->vertices.size();
    //totalTriangleCount += shape->indices.size() / 3;
    //
    //printf("Sphere vertice count: %d\n", totalVerticeCount);
    //printf("Sphere indice count: %d\n", totalIndiceCount);
    //printf("Sphere triangle count: %d\n", totalTriangleCount);
}

void SetVariables(Application& app)
{
    app.fixedFPS = 50;
    app.printFPS = true;
    app.currentScene->renderer->windowSize = Vector2(1080, 1080);
    app.currentScene->renderer->clearColor = Vector4(0.2, 0.3, 0.3, 1.0);
    app.currentScene->renderer->camera->cameraFOV = 50.0;
    app.currentScene->renderer->camera->size = Vector2(1080, 1080);
    app.currentScene->renderer->camera->position = Vector3(0, 0, 0);
}

int main()
{
    Application& app = *new Application();

    SceneBuilding(app);
    SetVariables(app);

    app.Run();

    return 0;
}
