#include "Encin/Encin.h"

namespace GamEncin
{
    vector<Mesh*> Renderer::meshes;
    Shader* Renderer::shaderProgram = nullptr;
    Camera* Renderer::mainCamera = nullptr;
    GLFWwindow* Renderer::window = nullptr;
    Vector2Int Renderer::windowSize = Vector2Int(1080, 1080);
    Vector4 Renderer::clearColor = Vector4(0.2f, 0.3f, 0.3f, 1.0f);
    bool  Renderer::isFullScreen = false,
        Renderer::vSyncEnabled = false,
        Renderer::windowCloseInput = false;

    void Renderer::InitialRender()
    {
        if(glfwInit() == GLFW_FALSE)
            Application::Stop(GLFWErr);

        // OpenGL Version : 460 core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(windowSize.x, windowSize.y, Application::GetProgramName().c_str(), NULL, NULL);

        if(!window)
            Application::Stop(GLFWErr);

        glfwMakeContextCurrent(window);

        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            Application::Stop(GLADErr);

        if(!gladLoadGL())
            Application::Stop(GLADErr);

        SetFullScreen(isFullScreen);

        SetVSync(vSyncEnabled);

        shaderProgram = new Shader("GamEncin/src/Shaders/vert.glsl", "GamEncin/src/Shaders/frag.glsl");

        glEnable(GL_DEPTH_TEST);

        for(Mesh* mesh : meshes)
        {
            mesh->Initialize();
        }
    }

    void Renderer::SetWindowProperties(bool newIsFullScreenNew, bool newVSyncEnabled, Vector2Int newWindowSize, Vector4 newClearColor)
    {
        isFullScreen = newIsFullScreenNew;
        clearColor = newClearColor;
        vSyncEnabled = newVSyncEnabled;
        windowSize = newWindowSize;

        if(Application::isRunning)
        {
            SetFullScreen(isFullScreen);
            SetVSync(vSyncEnabled);
        }
    }

    void Renderer::SetMainCamera(Camera* camera)
    {
        mainCamera = camera;
    }

    void Renderer::SetMainWindowSize(Vector2Int newWindowSize)
    {
        if(isFullScreen)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glViewport(0, 0, mode->width, mode->height);
        }
        else
        {
            windowSize = newWindowSize;
            glViewport(0, 0, windowSize.x, windowSize.y);
        }
    }

    void Renderer::SetFullScreen(bool value)
    {
        isFullScreen = value;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        if(isFullScreen)
        {
            printf("Monitor Resolution: %d x %d\n", mode->width, mode->height);
            printf("Monitor Refresh Rate: %d\n", mode->refreshRate);

            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else
        {
            glfwSetWindowMonitor(window, NULL, 100, 100, windowSize.x, windowSize.y, 0);
        }
    }

    void Renderer::SetVSync(bool value)
    {
        vSyncEnabled = value;
        glfwSwapInterval(vSyncEnabled ? 1 : 0);
    }

    bool Renderer::GetWindowCloseInput()
    {
        return windowCloseInput;
    }

    GLFWwindow* Renderer::GetMainWindow()
    {
        return window;
    }

    Vector2Int Renderer::GetMainWindowSize()
    {
        if(isFullScreen)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            return Vector2Int(mode->width, mode->height);
        }
        else
        {
            return windowSize;
        }
    }

    bool Renderer::IsFullScreen()
    {
        return isFullScreen;
    }

    void Renderer::AddMesh(Mesh* mesh)
    {
        meshes.push_back(mesh);
    }

    void Renderer::RemoveMesh(Mesh* mesh)
    {
        if(!mesh)
        {
            Application::Stop(NullPointerErr, "Mesh trying to remove is null");
            return;
        }

        auto obj = std::find(meshes.begin(), meshes.end(), mesh);
        if(obj != meshes.end())
        {
            meshes.erase(obj);
        }
        else
        {
            Application::Stop(ElementCouldNotFoundErr, "Couldn't found mesh to remove");
        }
    }

    void Renderer::RenderFrame()
    {
        shaderProgram->Use();

        for(Mesh* mesh : meshes)
        {
            Transform* transform = mesh->object->transform;

            GLSendUniformVector3(shaderProgram->objPositionVarId, transform->position);
            GLSendUniformVector3(shaderProgram->objScaleVarId, transform->scale);
            GLSendUniformVector3(shaderProgram->objRotationVarId, transform->rotation);

            mesh->Draw();
        }

        glfwSwapBuffers(window);

        mainCamera->UseCamera(shaderProgram->transformMatrixVarId);

        windowCloseInput = glfwWindowShouldClose(window);

        ClearColor(clearColor);

        glFinish();
    }

    void Renderer::ClearColor(Vector4 clearColor)
    {
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        SetMainWindowSize(Vector2Int(width, height));
    }

    void Renderer::GLSendUniformVector3(unsigned int& location, Vector3 vector3)
    {
        glUniform3f(location, vector3.x, vector3.y, vector3.z);
    }

    void Renderer::EndRenderer()
    {
        if(shaderProgram)
            shaderProgram->Delete();
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
