#include "Encin.h"

namespace GamEncin
{
#pragma region Object

    float* Object::VerticesVectorToFloatArr(vector<Vector3> vertices)
    {
        size_t size = vertices.size() * 3; // coordinate count
        float* result = new float[size];

        int j = 0;
        for(int i = 0; i < size; i += 3)
        {
            Vector3 currV3 = vertices.at(j);
            result[i] = currV3.x;
            result[i + 1] = currV3.y;
            result[i + 2] = currV3.z;
            j++;
        }

        return result;
    }

    void Object::SendVerticesDataToBuffer(vector<Vector3> vertices)
    {
        float* verticesFloatArr = VerticesVectorToFloatArr(vertices);

        size_t arrLength = vertices.size() * 3; // float count

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arrLength, verticesFloatArr, GL_DYNAMIC_DRAW);
        // copy the vertex data into the buffer's memory

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) 0);
        // attribute start pos, arrLength floats of data, not normalized, stride: 3 floats (for triangles), offset: 0

        glEnableVertexAttribArray(0); // enable the attribute at location 0
    }
#pragma endregion

#pragma region Scene

    Scene::Scene()
    {
        Application::GetInstance().scenes.push_back(this);
        renderer = new Renderer;
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
        renderer->InitialRender();
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

#pragma region Renderer

    void Renderer::InitialRender()
    {
        if(!glfwInit())
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
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        gladLoadGL();

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            Application::GetInstance().Stop(GLADErr); // Exit the function if GLAD initialization fails

        // In OpenGL, objects like VAOs, VBOs, shaders, and textures are handles or IDs that reference data stored in the GPU. So we use GLuint to store them.
        // lifecycle / pipeline of each object: creation -> binding -> configuration -> usage -> unbinding / deletion.


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        //generating vertice array and buffer objects

        // Binding makes an object the active one in the context (window). When we call a function, what it does depends on the internal state of opengl - on the context/object. There can be only one active object of each type at a time. fe: only one active VAO, VBO, texture, etc.
        glBindVertexArray(VAO); // set the VAO as the current Vertex Array Object
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // set the VBO as the current buffer object

        GLfloat vertices[9] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) 0);

        glEnableVertexAttribArray(0); // enable the attribute at location 0
    }

    void Renderer::RenderFrame()
    {
        glClearColor(0.5f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram->Use();
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        windowCloseInput = glfwWindowShouldClose(window);

        glfwPollEvents();
    }

    void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void Renderer::EndRenderer()
    {
        shaderProgram->Delete();
        glfwDestroyWindow(window);
        glfwTerminate();
    }

#pragma endregion 
}
