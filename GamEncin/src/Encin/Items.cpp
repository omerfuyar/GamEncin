#include "Encin.h"

namespace GamEncin
{
#pragma region Object

    Object::~Object()
    {
        if(vao)
            vao->Delete();
        if(vbo)
            vbo->Delete();
        if(ebo)
            ebo->Delete();
    }

    void Object::Draw()
    {
        vbo->Update(vertices);

        vao->Bind();
        vbo->Bind();
        ebo->Bind();

        vao->LinkAttirbutes(POSITION_VBO_LAYOUT, 3, GL_FLOAT, sizeof(Vector3), (void*) 0);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }

    void Object::Initialize()
    {
        vao = new VAO();
        vbo = new VBO(vertices);
        ebo = new EBO(indices);
        //size is coming from vectors, so it is the size of the vector in bytes
    }

#pragma endregion

#pragma region Scene

    Scene::Scene()
    {
        Application::GetInstance().scenes.push_back(this);
    }

    void Scene::AddObject(Object& object)
    {
        objects.push_back(&object);
    }

    void Scene::RemoveObject(Object& object)
    {
        auto obj = std::find(objects.begin(), objects.end(), &object);

        if(obj != objects.end())
        {
            objects.erase(obj);
        }
        else
        {
            Application::GetInstance().Stop(ObjCouldNotFoundErr);
        }
    }

    void Scene::Clear()
    {
        objects.clear();
    }

    void Scene::Awake()
    {
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
    }

    void Scene::Update()
    {
        for(Object* object : objects)
        {
            object->Update();
        }
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

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);

        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, fragmentShader);
        glLinkProgram(ID);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        //shaders already compiled and linked to the program, so we can delete them
    }

    void Shader::Use()
    {
        glUseProgram(ID);
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

    void VAO::LinkAttirbutes(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
    {
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        // layout: location of the vertex attribute in the shader
        //in shader, layout(location = 0) in vec3 position;
        glEnableVertexAttribArray(layout);
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

#pragma region Renderer

    void Renderer::InitialRender(vector<Object*> objects)
    {
        if(glfwInit() == GLFW_FALSE)
            Application::GetInstance().Stop(GLFWErr); // Exit the function if GLFW initialization fails

        // Configure the OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(640, 480, "GamEncin", NULL, NULL);

        if(!window)
            Application::GetInstance().Stop(GLFWErr); // Exit the function if window creation fails

        glfwMakeContextCurrent(window);

        //Registers a callback function that is called when the window is resized
        glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            Application::GetInstance().Stop(GLADErr); // Exit the function if GLAD initialization fails

        if(!gladLoadGL())
            Application::GetInstance().Stop(GLADErr);

        // In OpenGL, objects like VAOs, VBOs, shaders, and textures are handles or IDs that reference data stored in the GPU. So we use GLuint to store them.
        // lifecycle / pipeline of each object: creation -> binding -> configuration -> usage -> unbinding / deletion.

        // Binding makes an object the active one in the context (window). When we call a function, what it does depends on the internal state of opengl - on the context/object. There can be only one active object of each type at a time. fe: only one active VAO, VBO, texture, etc.

        shaderProgram = new Shader("GamEncin/src/Shaders/default.vert", "GamEncin/src/Shaders/default.frag");

        for(Object* object : objects)
        {
            object->Initialize();
        }
    }

    void Renderer::RenderFrame(vector<Object*> objects)
    {
        glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram->Use();

        for(Object* object : objects)
        {
            object->Draw();
        }

        //glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        windowCloseInput = glfwWindowShouldClose(window);

        glfwPollEvents();
    }

    void Renderer::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
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
