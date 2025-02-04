#include "Encin.h"
#include <iostream>

namespace GamEncin
{
#pragma region definitions

    GLFWwindow* window;

    GLuint shaderProgram, VBO, VAO; // Vertex Buffer Object, Vertex Array Object

    const char* vertexShaderSourceCode =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{ gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }";

    const char* fragmentShaderSourceCode =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{ FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f); }";

#pragma endregion

    void Application::Awake()
    {
        for(Scene* scene : scenes)
        {
            scene->Awake();
        }
    }

    void Application::Start()
    {
        for(Scene* scene : scenes)
        {
            scene->Start();
        }
    }

    void Application::Update()
    {
        for(Scene* scene : scenes)
        {
            scene->Update();
        }
    }

    void Application::FixUpdate()
    {
        for(Scene* scene : scenes)
        {
            scene->FixUpdate();
        }
    }

    void Application::RenderFrame()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    void Application::Run()
    {
        InitialRender();
        GameLoops();
    }

    void Application::InitialRender()
    {
        if(!glfwInit())
            End(GLFWErr); // Exit the function if GLFW initialization fails

        // Configure the OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(640, 480, "GamEncin", NULL, NULL);

        if(!window)
            End(GLFWErr); // Exit the function if window creation fails

        glfwMakeContextCurrent(window);

        //Registers a callback function that is called when the window is resized
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            End(GLADErr); // Exit the function if GLAD initialization fails

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

    void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void Application::GameLoops()
    {
        // Fixed Game loop
        const auto fixedDelay = 1000 / fixedFPS;

        // Others
        auto lastFixedUpdate = high_resolution_clock::now();
        int frameCount = 0;
        int fps = 0;
        long long msPastFromStart = 0;

        Awake();

        Start();

        while(!glfwWindowShouldClose(window))
        {
            Update();

            auto now = high_resolution_clock::now();
            auto fixElapsed = duration_cast<milliseconds>(now - lastFixedUpdate);

            if(fixElapsed.count() >= fixedDelay)
            {
                FixUpdate();
                lastFixedUpdate = now;
                msPastFromStart += fixedDelay;

                if(msPastFromStart % 1000 == 0)
                {
                    fps = frameCount;
                    frameCount = 0;
                    if(printFPS)
                        std::cout << fps << "\n"; //print fps
                }
            }

            if(FPSlimit != 0) // frame delay
            {
                const auto frameDelay = 1000 / FPSlimit;
                std::this_thread::sleep_for(milliseconds(frameDelay));
            }

            frameCount++;
        }

        End(Safe); // time to 31
    }

    void Application::End(EndType et)
    {
        switch(et)
        {
            case Safe:
                fprintf(stderr, "Program ended safely\n");
                break;
            case Warning:
                fprintf(stderr, "Program ended with warning(s)\n");
                break;
            case GLFWErr:
                fprintf(stderr, "ERROR: Error occurred in GLFW3\n");
                break;
            case GLADErr:
                fprintf(stderr, "ERROR: Error occurred in GLAD\n");
                break;
            case ObjCouldNotFind:
                fprintf(stderr, "ERROR: Object could not be found\n");
                break;
            default:
                fprintf(stderr, "ERROR: Unknown error occurred\n");
                break;
        }

        glfwTerminate();
        exit(et);
    }
}
