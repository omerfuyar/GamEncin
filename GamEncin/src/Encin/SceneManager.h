#pragma once
#include "Components.h"
#include "Tools.h"
#include <chrono>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <thread>
#include <vector>

using namespace std::chrono;

using std::vector;
using std::shared_ptr;
using std::unique_ptr;

namespace GamEncin
{
	class SceneManager //Objects Loop Manager
	{
	private:
		SceneManager() {}

	public:
		static vector<shared_ptr<Object>> objects; //Objects in the scene
		static unique_ptr<SceneManager> INSTANCE; //Singleton Instance

		//these can be moved to initial render function
		static const char* vertexShaderSourceCode;
		static const char* fragmentShaderSourceCode;

		static GLFWwindow* window;
		static GLuint shaderProgram, VBO, VAO; // Vertex Buffer Object, Vertex Array Object

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

#pragma region Scene Management Functions

		static SceneManager& GetInstance()
		{
			if(!INSTANCE)
			{
				INSTANCE.reset(new SceneManager());
			}
			return *INSTANCE;
		}

		static void AddObject(std::shared_ptr<GamEncin::Object> obj)
		{
			objects.push_back(obj);
		}

		static void Awake()
		{
			for(auto& obj : objects)
				obj->Awake();
		}

		static void Start()
		{
			for(auto& obj : objects)
				obj->Start();
		}

		static void Update()
		{
			for(auto& obj : objects)
				obj->Update();
		}

		static void FixUpdate()
		{
			for(auto& obj : objects)
				obj->FixUpdate();
		}

		static void End(int exitCode)
		{
			if(exitCode == -1) //GLFW Error
				fprintf(stderr, "ERROR: Error occurred in GLFW3\n");
			if(exitCode == -2) //GLAD Error
				fprintf(stderr, "ERROR: Error occurred in GLAD\n");

			glfwTerminate(); //Terminate GLFW
			exit(exitCode); //Exit the program
		}

#pragma endregion

#pragma region Rendering Functions

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		static void RenderFrame()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);

			glBindVertexArray(VAO);

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		static void SendVerticesDataToBuffer(vector<Vector3> vertices)
		{
			float* verticesArr = Vector3::VerticesVectorToFloatArr(vertices);
			size_t arrLength = vertices.size() * 3;
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * arrLength, verticesArr, GL_DYNAMIC_DRAW); 
			// copy the vertex data into the buffer's memory

			glVertexAttribPointer(0, arrLength, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
			// attribute start pos, 3 floats of data, not normalized, stride: 3 floats, offset: 0

			glEnableVertexAttribArray(0); // enable the attribute at location 0
		}

		static void InitialRender()
		{
			if(!glfwInit())
				End(-1); // Exit the function if GLFW initialization fails

			// Configure the OpenGL version
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			window = glfwCreateWindow(640, 480, "GamEncin", NULL, NULL);

			if(!window)
				End(-1); // Exit the function if window creation fails

			glfwMakeContextCurrent(window);

			//Registers a callback function that is called when the window is resized
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

			if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
				End(-2); // Exit the function if GLAD initialization fails

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

			//not mandatory, but unbinding VBO and VAO is a good practice
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			//glBindVertexArray(0);
		}

		static void GameLoops()
		{
#pragma region variable definitions

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

#pragma endregion

			Awake();

			Start();
			RenderFrame(); //render starts with start function

			while(!glfwWindowShouldClose(window))
			{
				Update();
				RenderFrame(); //rendering in update, not in fixupdate

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
	};
}