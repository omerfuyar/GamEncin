#include "Encin/Encin.h"

namespace GamEncin
{
    bool Renderer::isFullScreen = false;
    bool Renderer::vSyncEnabled = false;
    bool Renderer::windowCloseInput = false;

    Vector2Int Renderer::windowSize = Vector2Int(1080, 1080);
    Vector4 Renderer::clearColor = Vector4(0.2f, 0.3f, 0.3f, 1.0f);

    vector<Mesh*> Renderer::meshes;
    Shader* Renderer::shaderProgram = nullptr;
    Camera* Renderer::mainCamera = nullptr;
    GLFWwindow* Renderer::window = nullptr;

    GLArrayObject* Renderer::mainVAO = nullptr;
    GLBufferObject<RawVertex>* Renderer::modelVertexBO = nullptr;
    GLBufferObject<unsigned int>* Renderer::modelIndexBO = nullptr;
    GLShaderStorageBufferObject<Matrix4>* Renderer::modelMatrixSSBO = nullptr;
    GLShaderStorageBufferObject<unsigned long long>* Renderer::modelTextureHandlesSSBO = nullptr;

    vector<RawVertex> Renderer::batchedVertices;
    vector<unsigned int> Renderer::batchedIndices;
    vector<Matrix4> Renderer::batchedModelMatrices;
    vector<unsigned long long> Renderer::batchedTextureHandles;

    void Renderer::SetWindowProperties(bool newIsFullScreen, bool newVSyncEnabled, Vector2Int newWindowSize, Vector4 newClearColor)
    {
        isFullScreen = newIsFullScreen;
        clearColor = newClearColor;
        vSyncEnabled = newVSyncEnabled;
        windowSize = newWindowSize;

        if(Application::IsRunning())
        {
            SetFullScreen(isFullScreen);
            SetVSync(vSyncEnabled);
        }
    }

    void Renderer::SetMainCamera(Camera* camera)
    {
        if(!camera)
        {
            Application::PrintLog(NullPointerErr, "Camera trying to set as main is null");
            return;
        }

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

    GLFWwindow* const Renderer::GetMainWindow()
    {
        return window;
    }

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

        mesh->GetMeshData()->SetForBatch(meshes.size(), batchedVertices.size(), batchedIndices.size());
        vector<RawVertex> tempVertices = mesh->GetMeshData()->GetRawVertexArray();
        vector<unsigned int> tempIndices = mesh->GetMeshData()->GetIndiceArray();

        batchedVertices.insert(batchedVertices.end(), tempVertices.begin(), tempVertices.end());
        batchedIndices.insert(batchedIndices.end(), tempIndices.begin(), tempIndices.end());

        batchedModelMatrices.push_back(mesh->GetOwnerObject()->GetTransform()->GetModelMatrix());
        batchedTextureHandles.push_back(mesh->GetMeshTexture()->handle);

        meshes.push_back(mesh);
    }

    void Renderer::RemoveMesh(Mesh* mesh)
    {
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

        MeshData& meshData = *mesh->GetMeshData();

        auto vertexBeginIt = batchedVertices.begin() + meshData.batchVertexOffset;
        auto vertexEndIt = vertexBeginIt + meshData.vertices.size();
        batchedVertices.erase(vertexBeginIt, vertexEndIt);

        auto indexBeginIt = batchedIndices.begin() + meshData.batchIndexOffset;
        auto indexEndIt = indexBeginIt + (meshData.faces.size() * 3);
        batchedIndices.erase(indexBeginIt, indexEndIt);

        auto modelMatrixIt = batchedModelMatrices.begin() + meshData.id;
        batchedModelMatrices.erase(modelMatrixIt);

        auto textureHandleIt = batchedTextureHandles.begin() + meshData.id;
        batchedTextureHandles.erase(textureHandleIt);

        for(int i = meshData.id + 1; i < meshes.size(); i++)
        {
            Mesh* tempMesh = meshes[i];

            tempMesh->GetMeshData()->SetForBatch(
                i - 1,
                tempMesh->GetMeshData()->batchVertexOffset - meshData.vertices.size(),
                tempMesh->GetMeshData()->batchIndexOffset - (meshData.faces.size() * 3)
            );
        }

        for(int i = meshData.batchVertexOffset; i < batchedVertices.size(); i++)
        {
            batchedVertices[i].objectId--;
        }

        for(int i = meshData.batchIndexOffset; i < batchedIndices.size(); i++)
        {
            batchedIndices[i] -= meshData.vertices.size();
        }

        meshes.erase(obj);
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

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        //TODO for release, works
        //string exePath = Input::GetExeFilePath();
        //string vertShaderPath = exePath + "/vert.glsl";
        //string fragShaderPath = exePath + "/frag.glsl";
        //
        //shaderProgram = new Shader(vertShaderPath.c_str(), fragShaderPath.c_str());

        shaderProgram = new Shader("GamEncin/src/Encin/Shaders/vert.glsl", "GamEncin/src/Encin/Shaders/frag.glsl");

        mainVAO = new GLArrayObject(sizeof(RawVertex));
        modelVertexBO = new GLBufferObject<RawVertex>(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
        modelIndexBO = new GLBufferObject<unsigned int>(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
        modelMatrixSSBO = new GLShaderStorageBufferObject<Matrix4>(GE_SSBO_MODEL_MATRICES_BINDING, GL_STATIC_DRAW);
        modelTextureHandlesSSBO = new GLShaderStorageBufferObject<unsigned long long>(GE_SSBO_TEXTURE_HANDLES_BINDING, GL_STATIC_DRAW);

        LinkAttributes();

        TextureManager::InitializeTextures();
    }

    void Renderer::RenderFrame()
    {
        ClearColor(clearColor);

        shaderProgram->Use();

        mainCamera->UseCamera(shaderProgram->viewMatrixVarId, shaderProgram->projectionMatrixVarId);

        DrawBatchedMeshes();

        glfwSwapBuffers(window);

        windowCloseInput = glfwWindowShouldClose(window);

        glFlush();
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

    void Renderer::EndRenderer()
    {
        glFinish();
        glfwTerminate();
    }

    void Renderer::UpdateSSBOs()
    {
        for(Mesh* mesh : meshes)
        {
            int index = mesh->GetMeshData()->id;
            batchedModelMatrices[index] = mesh->GetOwnerObject()->GetTransform()->GetModelMatrix();
            batchedTextureHandles[index] = mesh->GetMeshTexture() ? mesh->GetMeshTexture()->handle : 0;
        }
    }

    void Renderer::DrawBatchedMeshes()
    {
        mainVAO->Bind();

        modelVertexBO->Update(batchedVertices);

        modelIndexBO->Update(batchedIndices);

        UpdateSSBOs();
        modelMatrixSSBO->Update(batchedModelMatrices);
        modelTextureHandlesSSBO->Update(batchedTextureHandles);

        glDrawElements(GL_TRIANGLES, batchedIndices.size(), GL_UNSIGNED_INT, 0);
    }

    void Renderer::ClearColor(Vector4 clearColor)
    {
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::LinkAttributes()
    {
        mainVAO->Bind();

        modelVertexBO->Bind();
        modelIndexBO->Bind();
        modelMatrixSSBO->Bind();
        modelTextureHandlesSSBO->Bind();

        unsigned int offset = 0;
        mainVAO->LinkIntegerAttribute(GE_VBO_OBJECT_ID_LAYOUT, 1, GL_UNSIGNED_INT, 0); //unsigned int
        offset += sizeof(unsigned int);
        mainVAO->LinkAttribute(GE_VBO_POSITION_LAYOUT, 3, GL_FLOAT, offset); //Vector3
        offset += sizeof(Vector3);
        mainVAO->LinkAttribute(GE_VBO_NORMAL_LAYOUT, 3, GL_FLOAT, offset); //Vector3
        offset += sizeof(Vector3);
        mainVAO->LinkAttribute(GE_VBO_UV_LAYOUT, 2, GL_FLOAT, offset); //Vector2
        offset += sizeof(Vector2);
    }

    void Renderer::GLSendUniformMatrix4(unsigned int& location, Matrix4 matrix4)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix4));
    }
}
