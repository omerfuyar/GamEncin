#include "ECS.h"

namespace GamEncin
{
#pragma region Application

	void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Application::InitialRender()
	{
		if(!glfwInit())
			systemManager.End(-1); // Exit the function if GLFW initialization fails

		// Configure the OpenGL version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(640, 480, "GamEncin", NULL, NULL);

		if(!window)
			systemManager.End(-1); // Exit the function if window creation fails

		glfwMakeContextCurrent(window);

		//Registers a callback function that is called when the window is resized
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
			systemManager.End(-2); // Exit the function if GLAD initialization fails

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

	void Application::Run()
	{
		InitialRender();
		systemManager.GameLoops();
	}

#pragma endregion

#pragma region System Manager

	void SystemManager::Awake()
	{
		transformManager.Awake();
		psychicsBodyManager.Awake();
		rendererManager.Awake();
	}

	void SystemManager::Start()
	{
		transformManager.Start();
		psychicsBodyManager.Start();
		rendererManager.Start();
	}

	void SystemManager::Update()
	{
		transformManager.Update();
		psychicsBodyManager.Update();
		rendererManager.Update();
	}

	void SystemManager::FixUpdate()
	{
		transformManager.FixUpdate();
		psychicsBodyManager.FixUpdate();
		rendererManager.FixUpdate();
	}

	void SystemManager::End(int exitCode)
	{
		if(exitCode == -1) //GLFW Error
			fprintf(stderr, "ERROR: Error occurred in GLFW3\n");
		if(exitCode == -2) //GLAD Error
			fprintf(stderr, "ERROR: Error occurred in GLAD\n");

		glfwTerminate(); //Terminate GLFW
		exit(exitCode); //Exit the program
	}
	
	void SystemManager::GameLoops()
	{
		// Frame Rate Limit
		const int FPSlimit = 100;
		const auto frameDelay = 1000 / FPSlimit;
		bool haveFPSlimit = false;

		// Fixed Game loop
		const int fixFPSlimit = 50;
		const auto fixedDelay = 1000 / fixFPSlimit;

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
					//std::cout << fps << "\n"; //print fps
				}
			}

			if(haveFPSlimit) // frame delay
				std::this_thread::sleep_for(milliseconds(frameDelay));

			frameCount++;
		}

		End(31); // time to 31
	}

#pragma endregion

#pragma region Transform Manager

	void TransformManager::Awake()
	{

	}

	void TransformManager::Start()
	{

	}

	void TransformManager::Update()
	{

	}

	void TransformManager::FixUpdate()
	{

	}

#pragma endregion

#pragma region Psychics Body Manager

	void PsychicsBodyManager::Awake()
	{

	}

	void PsychicsBodyManager::Start()
	{

	}

	void PsychicsBodyManager::Update()
	{

	}

	void PsychicsBodyManager::FixUpdate()
	{

	}

#pragma endregion

#pragma region Renderer Manager

	void RendererManager::RenderFrame()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	void RendererManager::SendVerticesDataToBuffer(vector<Vector3> vertices)
	{
		float* verticesFloatArr = Vector3::VerticesVectorToFloatArr(vertices);

		size_t arrLength = vertices.size() * 3; // float count

		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arrLength, verticesFloatArr, GL_DYNAMIC_DRAW);
		// copy the vertex data into the buffer's memory

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) 0);
		// attribute start pos, arrLength floats of data, not normalized, stride: 3 floats (for triangles), offset: 0

		glEnableVertexAttribArray(0); // enable the attribute at location 0
	}

	void RendererManager::Awake()
	{

	}

	void RendererManager::Start()
	{

	}

	void RendererManager::Update()
	{

	}

	void RendererManager::FixUpdate()
	{

	}

#pragma endregion
}