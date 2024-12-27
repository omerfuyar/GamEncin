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
	class Object; // Forward Declaration

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

		static SceneManager& GetInstance();

		static void AddObject(shared_ptr<Object> obj);

		static void Awake();

		static void Start();

		static void Update();

		static void FixUpdate();

		static void End(int exitCode);

#pragma endregion

#pragma region Rendering Functions

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

		static void RenderFrame();

		static void SendVerticesDataToBuffer(vector<Vector3> vertices);

		static void InitialRender();

		static void GameLoops();

#pragma endregion
	};
}
