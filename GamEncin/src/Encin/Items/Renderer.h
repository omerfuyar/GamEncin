#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/OpenGLObjects.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Camera;
    class Scene;
    class Mesh;
    struct Shader;

    class Renderer
    {
    public:
        static void SetWindowProperties(bool isFullScreen, bool vSyncEnabled, Vector2Int newWindowSize, Vector4 newClearColor);
        static void SetMainCamera(Camera *camera);
        static void SetMainWindowSize(Vector2Int newWindowSize);
        static void SetFullScreen(bool value);
        static void SetVSync(bool value);

        static bool GetWindowCloseInput();
        static bool IsFullScreen();
        static bool IsVSyncEnabled();
        static Vector2Int GetMainWindowSize();
        static GLFWwindow *const GetMainWindow();
        static Camera *const GetMainCamera();

        static void AddMeshesInScene(Scene *scene);
        static void ClearMeshes();
        static void AddMesh(Mesh *mesh);
        static void RemoveMesh(Mesh *mesh);
        static void InitialRender();
        static void RenderFrame();
        static void FrameBufferSizeCallback(GLFWwindow *window, int width, int height);
        static void EndRenderer();

    private:
        static bool isFullScreen;
        static bool vSyncEnabled;
        static bool windowCloseInput;

        static Vector2Int windowSize;
        static Vector4 clearColor;

        static vector<Mesh *> meshes;
        static Shader *shaderProgram;
        static Camera *mainCamera;
        static GLFWwindow *window;

        static GLArrayObject *mainVAO;

        static GLBufferObject<RawVertex> *modelVertexBO;
        static GLBufferObject<unsigned int> *modelIndexBO;

        static GLShaderStorageBufferObject<Matrix4> *modelMatrixSSBO;
        static GLShaderStorageBufferObject<unsigned long long> *modelTextureHandlesSSBO;

        static vector<RawVertex> batchedModelVertices;
        static vector<unsigned int> batchedModelIndices;

        static vector<Matrix4> batchedModelMatrices;
        static vector<unsigned long long> batchedTextureHandles;

        Renderer() = delete;
        Renderer(const Renderer &) = delete;
        void operator=(const Renderer &) = delete;

        static void UpdatePerMeshDatas();
        static void DrawBatchedMeshes();
        static void ClearColor(Vector4 clearColor);
        static void LinkAttributes();
        static void GLSendUniformMatrix4(unsigned int &location, Matrix4 matrix4);
    };
}
