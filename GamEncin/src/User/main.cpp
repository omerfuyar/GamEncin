#include "GamEncin.h"
#include <windows.h> // Include the header for DWORD definition

#define GE_SELECT_OPTIMUM_GPU

void SceneBuilding()
{
    TestScene1 *scene1 = new TestScene1("test1");

    Application::AddScene(scene1);

    TestScene2 *scene2 = new TestScene2("test2");
    Application::AddScene(scene2);

    Application::LoadScene(scene2);
}

void SetVariables()
{
    Application::SetFixedFPS(50);
    Application::SetPrintFPS(true);
    Application::SetProgramName("GamEncin");
    Renderer::SetWindowProperties(false, false, Vector2Int(1080, 720), Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

int main()
{
    SceneBuilding();
    SetVariables();

    Application::Run();

    return 0;
}

#ifdef GE_SELECT_OPTIMUM_GPU
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) DWORD AmdPowerXpressRequestHighPerformance = 0x00000001;
}
#endif
