#include "Encin.h"

namespace GamEncin
{

#pragma region Other

    string getFileContents(const char* fileName)
    {
        std::ifstream in(fileName, std::ios::binary);

        if(in)
        {
            string contents;
            in.seekg(0, std::ios::end);
            contents.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&contents[0], contents.size());
            in.close();
            return(contents);
        }
        Application::Stop(IOErr);
        return "";
    }

#pragma endregion

    namespace InputSystem
    {

#pragma region Mouse

        Vector2 Mouse::position = Vector2(0, 0),
            Mouse::positionDelta = Vector2(0, 0);

        int Mouse::scrollDelta = 0;

        void Mouse::Update(GLFWwindow* window)
        {
            double x, y;
            Vector2 temp = position;

            glfwGetCursorPos(window, &x, &y);
            position = Vector2(x, y);
            positionDelta = position - temp;
        }

        void Mouse::ScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
        {
            scrollDelta = yoffset;
            printf("Mouse scroll: %d\n", Input::mouse.scrollDelta);
        }

#pragma endregion

#pragma region Input

        GLFWwindow* Input::window = nullptr;
        Mouse Input::mouse;

        void Input::Initialize(GLFWwindow* _window)
        {
            window = _window;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

            glfwSetScrollCallback(window, mouse.ScrollCallBack);
            glfwSetKeyCallback(window, KeyCallBack);
        }

        void Input::UpdateInputs()
        {
            mouse.Update(window);
        }

        void Input::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
        {

        }

        bool Input::GetKey(Key key)
        {
            return glfwGetKey(window, key);
        }

        bool Input::GetKeyDown(Key key)
        {
            return false;
        }

        bool Input::GetKeyUp(Key key)
        {
            return false;
        }

        bool Input::GetMouseButton(MouseButton mouseButton)
        {
            return false;
        }

        bool Input::GetMouseButtonDown(MouseButton mouseButton)
        {
            return false;
        }

        bool Input::GetMouseButtonUp(MouseButton mouseButton)
        {
            return false;
        }

#pragma endregion

    }
}
