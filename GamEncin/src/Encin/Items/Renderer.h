#pragma once
#include "Encin/Tools/MathYaman.h"

#include "GLAD/glad.h"
#include "GLFW/glfw3.h"

namespace GamEncin
{
    class Camera;
    class Mesh;
    struct Shader;

    class Renderer
    {
    public:
        static void SetWindowProperties(bool isFullScreen, bool vSyncEnabled, Vector2Int newWindowSize, Vector4 newClearColor);
        static void SetMainCamera(Camera* camera);
        static void SetMainWindowSize(Vector2Int newWindowSize);
        static void SetFullScreen(bool value);
        static void SetVSync(bool value);

        static bool GetWindowCloseInput();
        static GLFWwindow* GetMainWindow();
        static Vector2Int GetMainWindowSize();
        static bool IsFullScreen();

        static void AddMesh(Mesh* mesh);
        static void RemoveMesh(Mesh* mesh);
        static void RenderFrame();
        static void InitialRender();
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
        static void EndRenderer();

    private:
        static vector<Mesh*> meshes;
        static Shader* shaderProgram;
        static Camera* mainCamera;
        static GLFWwindow* window;

        static Vector2Int windowSize;
        static Vector4 clearColor;
        static bool  isFullScreen,
            vSyncEnabled,
            windowCloseInput;

        static void GLSendUniformVector3(unsigned int& location, Vector3 vector3);
        static void ClearColor(Vector4 clearColor);
    };
}
