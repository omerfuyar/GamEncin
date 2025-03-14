#include "Encin.h"

namespace GamEncin
{
#pragma region Shape

    Shape::~Shape()
    {
        if(vao)
            vao->Delete();
        if(vbo)
            vbo->Delete();
        if(ebo)
            ebo->Delete();
    }

    void Shape::Draw()
    {
        vao->Bind();
        vbo->Bind();
        ebo->Bind();

        vao->LinkAttributes(POSITION_VBO_LAYOUT, 3, GL_FLOAT, 0);
        vao->LinkAttributes(COLOR_VBO_LAYOUT, 3, GL_FLOAT, sizeof(Vector3));

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    void Shape::Initialize()
    {
        vao = new VAO();
        vbo = new VBO(vertices);
        ebo = new EBO(indices);
    }

#pragma endregion

#pragma region Scene

    Scene::Scene()
    {
        renderer = new Renderer();
    }

    void Scene::AddObject(Object* object)
    {
        objects.push_back(object);
    }

    void Scene::RemoveObject(Object* object)
    {
        auto obj = std::find(objects.begin(), objects.end(), object);

        if(obj != objects.end())
        {
            objects.erase(obj);
        }
        else
        {
            Application::Stop(ObjCouldNotFoundErr);
        }
    }

    void Scene::Clear()
    {
        objects.clear();
    }

    void Scene::Awake()
    {
        renderer->InitialRender();

        for(Object* object : objects)
        {
            object->Awake();
        }
    }

    void Scene::Start()
    {
        for(Object* object : objects)
        {
            object->Start();
        }

        renderer->RenderFrame();
    }

    void Scene::Update()
    {
        for(Object* object : objects)
        {
            object->Update();
        }
    }

    void Scene::LateUpdate()
    {
        for(Object* object : objects)
        {
            object->LateUpdate();
        }

        renderer->RenderFrame();
    }

    void Scene::FixUpdate()
    {
        for(Object* object : objects)
        {
            object->FixUpdate();
        }
    }

#pragma endregion

#pragma region Shader

    // Constructor that build the Shader Program from 2 different shaders
    Shader::Shader(const char* vertexFile, const char* fragmentFile)
    {
        string vertexCode = getFileContents(vertexFile);
        string fragmentCode = getFileContents(fragmentFile);

        const char* vertexSource = vertexCode.c_str();
        const char* fragmentSource = fragmentCode.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //create the shader object
        glShaderSource(vertexShader, 1, &vertexSource, NULL); //assign code to shader object
        glCompileShader(vertexShader); //compile the shader to machine code
        CheckShaderErrors(vertexShader, "VERTEX");

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        CheckShaderErrors(fragmentShader, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);
        CheckShaderErrors(ID, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        //shaders already compiled and linked to the program, so we can delete them

        transformMatrixVarID = glGetUniformLocation(ID, "transformMatrix");
        objPositionVarID = glGetUniformLocation(ID, "objPosition");
        objRotationVarID = glGetUniformLocation(ID, "objRotation");
        objScaleVarID = glGetUniformLocation(ID, "objScale");
    }

    void Shader::CheckShaderErrors(GLuint shader, const char* type)
    {
        GLint hasCompiled;
        char infoLog[1024];

        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                Application::Stop(ShaderCompilationErr, infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                Application::Stop(ShaderLinkingErr, infoLog);
            }
        }
    }

    void Shader::Use()
    {
        glUseProgram(ID);

        //TODO bruh

    }

    void Shader::Delete()
    {
        glDeleteProgram(ID);
    }

#pragma endregion

#pragma region VBO

    VBO::VBO(vector<Vector3> vertices)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);
    }

    void VBO::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }

    void VBO::Update(vector<Vector3> vertices)
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);
    }

    void VBO::Delete()
    {
        glDeleteBuffers(1, &ID);
    }

#pragma endregion

#pragma region EBO

    EBO::EBO(vector<GLuint> indices)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    }

    void EBO::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    void EBO::Delete()
    {
        glDeleteBuffers(1, &ID);

    }

#pragma endregion

#pragma region VAO

    VAO::VAO()
    {
        glGenVertexArrays(1, &ID);
    }

    //one attribute is one piece of data that is passed to the vertex shader for one vertex, like position, color, normal, etc.
    void VAO::LinkAttributes(GLuint layout, GLuint numComponents, GLenum type, GLuint offsetInBytes)
    {
        GLsizei stride = 2 * sizeof(Vector3); //TODO position + color
        GLvoid* offsetVar = (GLvoid*) (offsetInBytes);

        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offsetVar);
        // layout: location of the vertex attribute in the shader, like 0 for position, 1 for color
        //in shader, layout(location = 0) in vec3 position;
        //numComponents: how many components does the attribute have, like 3 for position, 4 for color
        //type: type of the data, like GL_FLOAT for position, GL_UNSIGNED_INT for color
        //stride: size of the vertex data structure, like sizeof(Vertex)
        //offset: where the attribute starts in the vertex data structure, like color comes after position, so offset is the size of position
        glEnableVertexAttribArray(layout); //like binding the format we want to read buffer data
    }

    void VAO::Bind()
    {
        glBindVertexArray(ID);
    }

    void VAO::Delete()
    {
        glDeleteVertexArrays(1, &ID);
    }

#pragma endregion

#pragma region Camera

    void Camera::UseCamera(GLuint& transformMatrixLocation)
    {
        viewMatrix = glm::lookAt(position.ToGLMVec3(), (position + rotation).ToGLMVec3(), Vector3::Up().ToGLMVec3());;
        perspectiveMatrix = glm::perspective(glm::radians(cameraFOV), size.x / size.y, 0.1f, 100.0f);

        //glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix * viewMatrix));
        glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
    }

#pragma endregion

#pragma region Renderer

    Renderer::Renderer()
    {
        camera = new Camera();
    }

    void Renderer::InitialRender()
    {
        if(glfwInit() == GLFW_FALSE)
            Application::Stop(GLFWErr);

        // OpenGL Version : 460 core
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(initWindowSize.x, initWindowSize.y, "GamEncin", NULL, NULL);

        if(!window)
            Application::Stop(GLFWErr);

        glfwMakeContextCurrent(window);

        glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            Application::Stop(GLADErr);

        if(!gladLoadGL())
            Application::Stop(GLADErr);

        shaderProgram = new Shader("GamEncin/src/Shaders/default.vert", "GamEncin/src/Shaders/default.frag");

        glEnable(GL_DEPTH_TEST);

        for(Shape* shape : shapes)
        {
            shape->Initialize();
        }
    }

    void Renderer::RenderFrame()
    {
        ClearColor(clearColor);

        UseShader();

        for(Shape* shape : shapes)
        {
            GLSendUniformVector3(shaderProgram->objPositionVarID, shape->position);
            GLSendUniformVector3(shaderProgram->objScaleVarID, shape->scale);
            GLSendUniformVector3(shaderProgram->objRotationVarID, shape->rotation);

            shape->Draw();
        }

        glfwSwapBuffers(window);

        windowCloseInput = glfwWindowShouldClose(window);

        glfwPollEvents();
    }

    void Renderer::UseShader()
    {
        shaderProgram->Use();
        camera->UseCamera(shaderProgram->transformMatrixVarID);
    }

    void Renderer::ClearColor(Vector4 clearColor)
    {
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    //called when the window is resized
    void Renderer::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        Application::currentScene->renderer->camera->size = Vector2(width, height);
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

#pragma endregion 
}
