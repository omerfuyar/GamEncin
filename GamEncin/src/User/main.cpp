#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene1 = app.CreateAndUseScene();

    TestObject* object1 = new TestObject;

    scene1.AddObject(*object1);

    //TestObject& object1 = scene1.CreateObject<TestObject>();
}

int main(void)
{
    printf("%f", MathYaman::Abs(-1));

    Application& app = Application::GetInstance();

    SceneBuilding(app);

    app.Run();

    return 0;
}
