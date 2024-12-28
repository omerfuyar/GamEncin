#pragma once
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
	GLFWwindow* window;

	GLuint shaderProgram, VBO, VAO; // Vertex Buffer Object, Vertex Array Object

	class TransformManager
	{
	public:
		void Awake();
		void Start();
		void Update();
		void FixUpdate();

	};

	class PsychicsBodyManager
	{
	public:
		void Awake();
		void Start();
		void Update();
		void FixUpdate();

	};

	class RendererManager
	{
	public:
		void Awake();
		void Start();
		void Update();
		void FixUpdate();

		void RenderFrame();
		void SendVerticesDataToBuffer(vector<Vector3> vertices);
	};

	class SystemManager
	{
	public:
		TransformManager transformManager;
		PsychicsBodyManager psychicsBodyManager;
		RendererManager rendererManager;

		void Awake();
		void Start();
		void Update();
		void FixUpdate();

		void End(int exitCode);
		void GameLoops();
	}; 
	
	class Application
	{
	public:
		SystemManager systemManager;

		const char* vertexShaderSourceCode =
			"#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}\0";

		const char* fragmentShaderSourceCode =
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
			"}\n\0";

		void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		void InitialRender();

		void Run();
	};
}