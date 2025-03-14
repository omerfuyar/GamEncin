#include "GamEncin.h"

void SceneBuilding()
{
    Scene& scene = Application::CreateAndUseScene();

    int totalVerticeCount = 0,
        totalIndiceCount = 0,
        totalTriangleCount = 0;

    vector<Shape*> shapes =
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
    float gap = 3.0;
    for(int i = 0; i < shapes.size(); i++)
    {
        scene.AddObject(shapes[i]);

        int row = i / gridCols;
        int col = i % gridCols;
        shapes[i]->position = Vector3((col - gridCols / 2), (row - gridRows / 2), -5) * gap;

        totalIndiceCount += shapes[i]->indices.size();
        totalVerticeCount += shapes[i]->vertices.size();
        totalTriangleCount += shapes[i]->indices.size() / 3;
    }

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

    printf("Sphere vertice count: %d\n", totalVerticeCount);
    printf("Sphere indice count: %d\n", totalIndiceCount);
    printf("Sphere triangle count: %d\n", totalTriangleCount);
}

void SetVariables()
{
    Application::fixedFPS = 50;
    Application::printFPS = true;
    Application::currentScene->renderer->initWindowSize = Vector2(1080, 1080);
    Application::currentScene->renderer->clearColor = Vector4(0.2, 0.3, 0.3, 1.0);
    Application::currentScene->renderer->camera->cameraFOV = 50.0;
    Application::currentScene->renderer->camera->size = Vector2(1080, 1080);
    Application::currentScene->renderer->camera->position = Vector3(0, 0, 0);
}

int main()
{
    SceneBuilding();
    SetVariables();

    Application::Run();

    return 0;
}
