#include "Encin.h"

namespace GamEncin
{
#pragma region Object

    void Object::SendVerticesDataToBuffer(vector<Vector3> vertices)
    {
        float* verticesFloatArr = Vector3::VerticesVectorToFloatArr(vertices);

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
            renderer->InitialRender();
        }
    }

    void Scene::Start()
    {
        for(Object* object : objects)
        {
            object->Start();
            renderer->RenderFrame();
        }
    }

    void Scene::Update()
    {
        for(Object* object : objects)
        {
            object->Update();
            renderer->RenderFrame();
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

#pragma region Renderer

    void Renderer::RenderFrame()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        //glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    void Renderer::InitialRender()
    {
        if(!glfwInit())
            Application::GetInstance().Stop(GLFWErr); // Exit the function if GLFW initialization fails

        // Configure the OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(640, 480, "GamEncin", NULL, NULL);

        if(!window)
            Application::GetInstance().Stop(GLFWErr); // Exit the function if window creation fails

        glfwMakeContextCurrent(window);

        //Registers a callback function that is called when the window is resized
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            Application::GetInstance().Stop(GLADErr); // Exit the function if GLAD initialization fails

        // In OpenGL, objects like VAOs, VBOs, shaders, and textures are handles or IDs that reference data stored in the GPU. So we use GLuint to store them.
        // lifecycle / pipeline of each object: creation -> binding -> configuration -> usage -> unbinding / deletion.

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGenVertexArrays(1, &VAO);	// Stores vertex attribute configurations
        glGenBuffers(1, &VBO); // Stores vertex data in GPU memory

        // Binding makes an object the active one in the context (window). When we call a function, what it does depends on the internal state of opengl - on the context/object. There can be only one active object of each type at a time. fe: only one active VAO, VBO, texture, etc.
        glBindVertexArray(VAO); // set the VAO as the current Vertex Array Object
        glBindBuffer(GL_ARRAY_BUFFER, VBO); // set the VBO as the current buffer object
    }

    void Renderer::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

#pragma endregion 
}
