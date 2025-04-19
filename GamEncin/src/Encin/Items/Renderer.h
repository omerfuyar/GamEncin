#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/OpenGLObjects.h"

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
        static void AddMesh(Mesh* mesh);
        static void RemoveMesh(Mesh* mesh);
        static void InitialRender();
        static void RenderFrame();
        static void EndRenderer();
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

        static void SetWindowProperties(bool isFullScreen, bool vSyncEnabled, Vector2Int newWindowSize, Vector4 newClearColor);
        static void SetMainCamera(Camera* camera);
        static void SetMainWindowSize(Vector2Int newWindowSize);
        static void SetFullScreen(bool value);
        static void SetVSync(bool value);

        static GLFWwindow* GetMainWindow();
        static Vector2Int GetMainWindowSize();
        static bool GetWindowCloseInput();
        static bool IsFullScreen();
        static bool IsVSyncEnabled();

    private:
        static vector<Mesh*> meshes;
        static Shader* shaderProgram;
        static Camera* mainCamera;
        static GLFWwindow* window;

        static VAO* mainVAO;
        static VBO* modelVertexVBO;
        static IBO* modelIndexIBO;
        static SSBO* modelMatrixSSBO;

        static vector<RawVertex> batchedVertices;
        static vector<unsigned int> batchedIndices;
        static vector<Matrix4*> batchedModelMatrices;

        static Vector2Int windowSize;
        static Vector4 clearColor;

        static bool isFullScreen;
        static bool vSyncEnabled;
        static bool windowCloseInput;

        static void GLSendUniformMatrix4(unsigned int& location, Matrix4 matrix4);
        static void ClearColor(Vector4 clearColor);
        static void DrawBatchedMeshes();
        static void LinkAttributes();
    };
}
