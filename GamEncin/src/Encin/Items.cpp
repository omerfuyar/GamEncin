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
        UpdateVertices(); //in case of data change

        vbo->Update(vertices);

        vao->Bind();
        vbo->Bind();
        ebo->Bind();

        vao->LinkAttributes(POSITION_VBO_LAYOUT, 3, GL_FLOAT, 0);
        vao->LinkAttributes(COLOR_VBO_LAYOUT, 3, GL_FLOAT, sizeof(Vector3));

        glDrawElements(GL_TRIANGLES, modelIndices.size(), GL_UNSIGNED_INT, 0);
    }

    void Object::UpdateVertices()
    {

        for(int i = 0; i < modelVertices.size(); i += 2) //for position, model to world position
            vertices[i] = position + modelVertices[i];

        for(int i = 1; i < modelVertices.size(); i += 2) //for color, 8bit to 0-1 float
            vertices[i] = modelVertices[i] / 255;

        //for(Vector3 vec : vertices)
        //{
        //    printf("%f, %f, %f\n", vec.x, vec.y, vec.z);
        //
        //}
    }

    void Object::Initialize()
    {
        for(int i = 0; i < modelVertices.size(); i++)
            vertices.push_back(modelVertices[i]); //change when color added

        vao = new VAO();
        vbo = new VBO(vertices);
        ebo = new EBO(modelIndices);
        //size is coming from vectors, so it is the size of the type times vector size
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

        scaleVarID = glGetUniformLocation(ID, "scale");
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

    //one attribute is one piece of data that is passed to the vertex shader for one vertex, like position, color, normal, etc.
    void VAO::LinkAttributes(GLuint layout, GLuint numComponents, GLenum type, GLuint offsetInBytes)
    {
        GLsizeiptr stride = sizeof(Vector3) * 2;
        void* offsetVar = (void*) (offsetInBytes);

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
            glUniform3f(shaderProgram->scaleVarID, object->scale.x, object->scale.y, object->scale.z);
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
