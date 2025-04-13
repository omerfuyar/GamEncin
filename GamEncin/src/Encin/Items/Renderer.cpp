#include "Encin/Encin.h"

namespace GamEncin
{
    unordered_map<unsigned int, Mesh*> Renderer::meshes;
    Shader* Renderer::shaderProgram = nullptr;
    Camera* Renderer::mainCamera = nullptr;
    GLFWwindow* Renderer::window = nullptr;

    VAO* Renderer::mainVAO = nullptr;
    VBO* Renderer::modelVertexVBO = nullptr;
    IBO* Renderer::modelIndexIBO = nullptr;
    SSBO* Renderer::modelMatrixSSBO = nullptr;

    Vector2Int Renderer::windowSize = Vector2Int(1080, 1080);
    Vector4 Renderer::clearColor = Vector4(0.2f, 0.3f, 0.3f, 1.0f);
    vector<RawVertex> Renderer::batchedVertices;
    vector<unsigned int> Renderer::batchedIndices;
    vector<Matrix4> Renderer::batchedModelMatrices;

    bool  Renderer::isFullScreen = false,
        Renderer::vSyncEnabled = false,
        Renderer::windowCloseInput = false;

    void Renderer::AddMesh(Mesh* mesh)
    {
        if(!mesh)
        {
            Application::Stop(NullPointerErr, "Mesh trying to add is null");
            return;
        }

        //auto obj = std::find(meshes.begin(), meshes.end(), mesh);
        //
        //if(obj != meshes.end())
        //{
        //    Application::Stop(ElementDuplicationErr, "Mesh already exists");
        //    return;
        //}

        meshes[meshes.size()] = mesh;
    }

    void Renderer::RemoveMesh(Mesh* mesh)
    {
        if(!mesh)
        {
            Application::Stop(NullPointerErr, "Mesh trying to remove is null");
            return;
        }

        //auto obj = std::find(meshes.begin(), meshes.end(), mesh);
        //if(obj != meshes.end())
        //{
        //    meshes.erase(obj);
        //}
        //else
        //{
        //    Application::Stop(ElementCouldNotFoundErr, "Couldn't found mesh to remove");
        //}
    }

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

        mainVAO = new VAO(sizeof(RawVertex) + sizeof(unsigned int));
        modelVertexVBO = new VBO();
        modelIndexIBO = new IBO();
        modelMatrixSSBO = new SSBO();

        LinkAttributes();
    }

    void Renderer::RenderFrame()
    {
        ClearColor(clearColor);

        shaderProgram->Use();

        mainCamera->UseCamera(shaderProgram->viewMatrixVarId, shaderProgram->projectionMatrixVarId);

        UpdateBatchedVerticesAndIndices();

        DrawBatchedMeshes();

        glfwSwapBuffers(window);

        windowCloseInput = glfwWindowShouldClose(window);

        //glFinish();
        //glFlush();
    }

    void Renderer::EndRenderer()
    {
        if(shaderProgram)
            shaderProgram->Delete();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Renderer::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        if(isFullScreen)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glViewport(0, 0, mode->width, mode->height);
        }
        else
        {
            SetMainWindowSize(Vector2Int(width, height));
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
        windowSize = newWindowSize;
        glViewport(0, 0, windowSize.x, windowSize.y);
    }

    void Renderer::SetFullScreen(bool value)
    {
        isFullScreen = value;

        if(isFullScreen)
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
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

    bool Renderer::GetWindowCloseInput()
    {
        return windowCloseInput;
    }

    bool Renderer::IsFullScreen()
    {
        return isFullScreen;
    }

    bool Renderer::IsVSyncEnabled()
    {
        return vSyncEnabled;
    }

    void Renderer::GLSendUniformMatrix4(unsigned int& location, Matrix4 matrix4)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix4));
    }

    void Renderer::ClearColor(Vector4 clearColor)
    {
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::UpdateBatchedVerticesAndIndices()
    {
        batchedVertices.clear();

        batchedIndices.clear();

        batchedModelMatrices.clear();
        batchedModelMatrices.resize(meshes.size());

        for(auto& pair : meshes)
        {
            Mesh* mesh = pair.second;

            for(RawVertex* vertex : mesh->meshData.vertices)
            {
                if(!vertex)
                {
                    Application::Stop(NullPointerErr, "Vertex is null");
                    return;
                }

                vertex->SetObjectId(pair.first); //set objectId to each vertex, ready to use now
            }

            //append matrices to main batch
            batchedModelMatrices[pair.first] = mesh->object->transform->GetModelMatrix();
            //todo optimize this, do not compute each time getting world matrix

            vector<RawVertex> tempVertices = mesh->meshData.GetRawVertexArray();
            vector<unsigned int> tempIndices = mesh->meshData.GetIndiceArray();

            //append vertices and indices to main batch
            batchedVertices.insert(batchedVertices.end(), tempVertices.begin(), tempVertices.end());
            batchedIndices.insert(batchedIndices.end(), tempIndices.begin(), tempIndices.end());
        }

        modelVertexVBO->Update(batchedVertices);
        modelIndexIBO->Update(batchedIndices);
        modelMatrixSSBO->Update(batchedModelMatrices);
    }

    void Renderer::DrawBatchedMeshes()
    {
        //binding has been done UpdateBatchedVerticesAndIndices

        glDrawElements(GL_TRIANGLES, batchedIndices.size(), GL_UNSIGNED_INT, 0);
    }

    void Renderer::LinkAttributes()
    {
        mainVAO->LinkIntegerAttribute(VBO_OBJECT_ID_LAYOUT, 1, GL_UNSIGNED_INT, 0); //unsigned int
        mainVAO->LinkAttribute(VBO_POSITION_LAYOUT, 3, GL_FLOAT, sizeof(unsigned int)); //Vector3
        mainVAO->LinkAttribute(VBO_COLOR_LAYOUT, 4, GL_FLOAT, sizeof(unsigned int) + sizeof(Vector3)); //Vector4
    }
}
