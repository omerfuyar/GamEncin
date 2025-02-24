#include "GamEncin.h"

void SceneBuilding(Application& app)
{
    Scene& scene1 = app.CreateAndUseScene();

    TestObject* object1 = new TestObject;
    object1->name = "Test Object 1";
    std::cout << object1->name << std::endl;
    scene1.AddObject(*object1);
    //both works fine
    //TestObject& object1 = scene1.CreateObject<TestObject>();
}

int main(void)
{
    Application& app = Application::GetInstance();

    SceneBuilding(app);

    app.Run();

    return 0;
}
