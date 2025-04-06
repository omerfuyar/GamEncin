#include "Encin/Encin.h"

namespace GamEncin
{
    void Renderer::InitialRender()
    {
        if(glfwInit() == GLFW_FALSE)
            Application::Stop(GLFWErr);

        // OpenGL Version : 460 core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(initWindowSize.x, initWindowSize.y, Application::programName.c_str(), NULL, NULL);

        if(!window)
            Application::Stop(GLFWErr);

        glfwMakeContextCurrent(window);

        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            Application::Stop(GLADErr);

        if(!gladLoadGL())
            Application::Stop(GLADErr);

        shaderProgram = new Shader("GamEncin/src/Shaders/vert.glsl", "GamEncin/src/Shaders/frag.glsl");

        glEnable(GL_DEPTH_TEST);

        for(Mesh* mesh : meshes)
        {
            mesh->Initialize();
        }
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
        ClearColor(clearColor);

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
    }

    void Renderer::ClearColor(Vector4 clearColor)
    {
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //called when the window is resized
    void Renderer::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        Application::currentScene->renderer->mainCamera->size = Vector2(width, height);
        glViewport(0, 0, width, height);
    }

    void Renderer::GLSendUniformVector3(GLuint& location, Vector3 vector3)
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
