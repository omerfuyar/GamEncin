#pragma once
#include "Components.h"
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
		static GLuint shaderProgram, VAO; // Vertex Array Object

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

		static void AddObject(shared_ptr<Object> obj)
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

		static void RenderFrame(GLFWwindow* window)
		{
			glfwPollEvents(); 

			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);

			glBindVertexArray(VAO);

			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwSwapBuffers(window);
		}

		static void InitialRender()
		{
			if(!glfwInit())
				End(-1); // Exit the function if GLFW initialization fails

			// Configure the OpenGL version
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

			if(!window)
				End(-1); // Exit the function if window creation fails

			glfwMakeContextCurrent(window);

			if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
				End(-2); // Exit the function if GLAD initialization fails

			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
			glCompileShader(vertexShader);

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
			glCompileShader(fragmentShader);

			GLuint shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			GLfloat vertices[] = {
				-0.5f, -0.5f, 0.0f,
				0.5f, -0.5f, 0.0f,
				0.0f, 0.5f, 0.0f
			};

			GLuint VBO; //Vertex Buffer Object

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
			// position attribute, 3 floats, not normalized, stride 3 floats, offset 0
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
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
			RenderFrame(window); //render starts with start function

			while(!glfwWindowShouldClose(window))
			{
				Update();
				RenderFrame(window); //rendering in update, not in fixupdate

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