#include "Encin/Encin.h"

namespace GamEncin
{
    vector<Mesh*> Renderer::meshes;
    Shader* Renderer::shaderProgram = nullptr;
    Camera* Renderer::mainCamera = nullptr;
    GLFWwindow* Renderer::window = nullptr;

    VAO* Renderer::mainVAO = nullptr;
    VBO* Renderer::modelVertexVBO = nullptr;
    IBO* Renderer::modelIndexIBO = nullptr;
    SSBO* Renderer::modelMatrixSSBO = nullptr;

    vector<RawVertex> Renderer::batchedVertices;
    vector<unsigned int> Renderer::batchedIndices;
    vector<Matrix4*> Renderer::batchedModelMatrices;

    Vector2Int Renderer::windowSize = Vector2Int(1080, 1080);
    Vector4 Renderer::clearColor = Vector4(0.2f, 0.3f, 0.3f, 1.0f);

    bool Renderer::isFullScreen = false;
    bool Renderer::vSyncEnabled = false;
    bool Renderer::windowCloseInput = false;

    void Renderer::AddMesh(Mesh* mesh)
    {
        if(!mesh)
        {
            Application::PrintLog(NullPointerErr, "Mesh trying to add is null");
            return;
        }

        auto obj = std::find(meshes.begin(), meshes.end(), mesh);

        if(obj != meshes.end())
        {
            Application::PrintLog(ElementDuplicationErr, "Mesh already exists in the renderer");
            return;
        }

        mesh->meshData.SetForBatch(batchedModelMatrices.size(), batchedVertices.size(), batchedVertices.size() + batchedIndices.size());
        vector<RawVertex> tempVertices = mesh->meshData.GetRawVertexArray();
        vector<unsigned int> tempIndices = mesh->meshData.GetIndiceArray();

        batchedVertices.insert(batchedVertices.end(), tempVertices.begin(), tempVertices.end());
        batchedIndices.insert(batchedIndices.end(), tempIndices.begin(), tempIndices.end());

        batchedModelMatrices.push_back(mesh->object->transform->GetModelMatrix());

        meshes.push_back(mesh);
    }

    void Renderer::RemoveMesh(Mesh* mesh)
    {
        printf("\nid to remove : %d\n", mesh->meshData.id);
        printf("\nmesh ids before removal : ");
        for(Mesh* mesh : meshes)
        {
            printf("%d, ", mesh->meshData.id);
        }
        printf("\n");

        printf("batched vertices size : %d\n", batchedVertices.size());
        printf("batched indices size : %d\n", batchedIndices.size());
        printf("batched matrices size : %d\n", batchedModelMatrices.size());

        if(!mesh)
        {
            Application::PrintLog(NullPointerErr, "Mesh trying to remove is null");
            return;
        }

        auto obj = std::find(meshes.begin(), meshes.end(), mesh);

        if(obj == meshes.end())
        {
            Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found mesh to remove");
            return;
        }

        auto vertexBeginIt = batchedVertices.begin() + mesh->meshData.batchVertexOffset;
        auto vertexEndIt = vertexBeginIt + mesh->meshData.vertices.size();
        batchedVertices.erase(vertexBeginIt, vertexEndIt);

        auto indexBeginIt = batchedIndices.begin() + mesh->meshData.batchIndexOffset;
        auto indexEndIt = indexBeginIt + (mesh->meshData.faces.size() * 3);
        batchedIndices.erase(indexBeginIt, indexEndIt);

        auto modelMatrixIt = batchedModelMatrices.begin() + mesh->meshData.id;
        batchedModelMatrices.erase(modelMatrixIt);

        for(int i = mesh->meshData.id + 1; i < meshes.size(); i++)
        {
            Mesh* tempMesh = meshes[i];
            tempMesh->meshData.SetForBatch(i - 1,
                                           tempMesh->meshData.batchVertexOffset - mesh->meshData.vertices.size(),
                                           tempMesh->meshData.batchIndexOffset - (mesh->meshData.faces.size() * 3));
        }

        meshes.erase(obj);

        printf("\nmesh ids after removal : ");
        for(Mesh* mesh : meshes)
        {
            printf("%d, ", mesh->meshData.id);
        }
        printf("\n");
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

        //TODO for release
        //string exePath = Input::GetExeFilePath();
        //string vertShaderPath = exePath + "/vert.glsl";
        //string fragShaderPath = exePath + "/frag.glsl";
        //
        //shaderProgram = new Shader(vertShaderPath.c_str(), fragShaderPath.c_str());

        shaderProgram = new Shader("GamEncin/src/Shaders/vert.glsl", "GamEncin/src/Shaders/frag.glsl");

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        mainVAO = new VAO(sizeof(RawVertex));
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

        DrawBatchedMeshes();

        glfwSwapBuffers(window);

        windowCloseInput = glfwWindowShouldClose(window);

        //glFinish();
        glFlush();
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

    void Renderer::DrawBatchedMeshes()
    {
        mainVAO->Bind();

        modelVertexVBO->Update(batchedVertices);
        modelIndexIBO->Update(batchedIndices);
        modelMatrixSSBO->Update(batchedModelMatrices);

        glDrawElements(GL_TRIANGLES, batchedIndices.size(), GL_UNSIGNED_INT, 0);
    }

    void Renderer::LinkAttributes()
    {
        mainVAO->Bind();

        modelVertexVBO->Bind();
        modelIndexIBO->Bind();
        modelMatrixSSBO->Bind();

        mainVAO->LinkIntegerAttribute(VBO_OBJECT_ID_LAYOUT, 1, GL_UNSIGNED_INT, 0); //unsigned int
        mainVAO->LinkAttribute(VBO_POSITION_LAYOUT, 3, GL_FLOAT, sizeof(unsigned int)); //Vector3
        mainVAO->LinkAttribute(VBO_COLOR_LAYOUT, 4, GL_FLOAT, sizeof(unsigned int) + sizeof(Vector3)); //Vector4
    }
}
