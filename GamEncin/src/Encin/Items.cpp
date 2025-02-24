#include "Encin.h"

namespace GamEncin
{
#pragma region Object

    Object::~Object()
    {
        if(positionVBO)
            positionVBO->Delete();
        if(positionEBO)
            positionEBO->Delete();
    }

    void Object::SetVerticeArr(vector<Vector3> vertices, GLfloat(&targetArr)[9])
    { //only for triangle
        int j = 0;
        for(int i = 0; i < 9; i += 3)
        {
            Vector3 currV3 = vertices.at(j);
            targetArr[i] = (GLfloat) currV3.x;
            targetArr[i + 1] = (GLfloat) currV3.y;
            targetArr[i + 2] = (GLfloat) currV3.z;
            j++;
        }
    }

    void Object::SendDataToBuffer()
    {
        SetVerticeArr(vertices, verticeArr);
        positionVBO = new VBO(verticeArr, sizeof(verticeArr));
        positionEBO = new EBO(indicesArr, sizeof(indicesArr));
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

    VBO::VBO(GLfloat* vertices, GLsizeiptr size)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void VBO::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }

    void VBO::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VBO::Delete()
    {
        glDeleteBuffers(1, &ID);
    }

#pragma endregion

#pragma region EBO

    EBO::EBO(GLuint* indices, GLsizeiptr size)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); // set the VBO as the current buffer object
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    }

    void EBO::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    }

    void EBO::Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
        glBindVertexArray(ID);
    }

    void VAO::LinkAttirbutes(VBO vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
    {
        vbo.Bind();
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        // layout: location of the vertex attribute in the shader
        //in shader, layout(location = 0) in vec3 position;
        glEnableVertexAttribArray(layout);
        vbo.Unbind();
    }

    void VAO::Bind()
    {
        glBindVertexArray(ID);
    }

    void VAO::Unbind()
    {
        glBindVertexArray(0);
    }

    void VAO::Delete()
    {
        glDeleteVertexArrays(1, &ID);
    }

#pragma endregion

#pragma region Renderer

    void Renderer::InitialRender()
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

        GLfloat vertices[] =
        {
            -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
            0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
            0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
            -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
            0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
            0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
        };

        // Indices for vertices order
        GLuint indices[] =
        {
            0, 3, 5, // Lower left triangle
            3, 2, 4, // Lower right triangle
            5, 4, 1 // Upper triangle
        };

        shaderProgram = new Shader("GamEncin/src/Shaders/default.vert", "GamEncin/src/Shaders/default.frag");

        mainVAO = new VAO();

        Object* obj = new Object;
        //Application::GetInstance().currentScene->AddObject(*obj);
        obj->vertices = {
            Vector3(0.5f, 0.5f, 0.0f),
            Vector3(0.5f, -0.5f, 0.0f),
            Vector3(-0.5f, -0.5f, 0.0f)
        };
        obj->SendDataToBuffer();
        mainVAO->LinkAttirbutes(*obj->positionVBO, POSITION_VBO_LAYOUT, 3, GL_FLOAT, 3 * sizeof(float), (void*) 0);
    }

    void Renderer::RenderFrame()
    {
        glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram->Use();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

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
        if(mainVAO)
            mainVAO->Delete();
        if(shaderProgram)
            shaderProgram->Delete();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

#pragma endregion 
}
