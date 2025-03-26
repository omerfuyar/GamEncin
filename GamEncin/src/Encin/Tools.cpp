#include "Encin.h"

namespace GamEncin
{
    namespace ToolKit
    {
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
    }

    namespace MathYaman
    {

    }

    namespace InputSystem
    {
        //TODO Be careful with the order of the keys and adding new keys, these arrays should be identical with the enum type in Tools.h
        KeyCode keyCodeArr[] = {
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

            Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

            Space, Enter, BackSpace, Escape, Tab, LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt, CapsLock, UpArrow, DownArrow, LeftArrow, RightArrow,

            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

            MouseLeft, MouseRight, MouseMiddle
        };

#pragma region Mouse

        Vector2 Mouse::position = Vector2(0, 0),
            Mouse::positionDelta = Vector2(0, 0);
        int Mouse::scrollDelta = 0;

        void Mouse::Initialize(GLFWwindow* window)
        {
            glfwSetScrollCallback(window, MouseScrollCallBack);
        }

        void Mouse::Update(GLFWwindow* window)
        {
            double x, y;
            Vector2 temp = position;

            glfwGetCursorPos(window, &x, &y);
            position = Vector2(x, y);
            positionDelta = position - temp;

            //resetting the value
            scrollDelta = 0;
        }

        void Mouse::MouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
        {
            scrollDelta = yoffset;
        }

#pragma endregion

#pragma region Input

        GLFWwindow* Input::window = nullptr;
        Mouse Input::mouse;
        map<int, KeyButtonStatus> Input::keys;

        void Input::Initialize(GLFWwindow* _window)
        {
            window = _window;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

            glfwSetMouseButtonCallback(window, MouseButtonCallBack);
            glfwSetKeyCallback(window, KeyCallBack);

            for(KeyCode code : keyCodeArr)
            {
                keys[code] = Released;
            }

            mouse.Initialize(window);
        }

        void Input::UpdateInputs()
        {
            //resetting values
            for(auto& key : keys)
            {
                switch(key.second)
                {
                    case Up:
                        key.second = Released;
                        break;
                    case Down:
                        key.second = Pressed;
                        break;
                    default:
                        continue;
                }
            }

            mouse.Update(window);

            glfwPollEvents();
        }

        void Input::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            switch(action)
            {
                case GLFW_PRESS:
                    keys[key] = Down;
                    break;
                case GLFW_RELEASE:
                    keys[key] = Up;
                    break;
            }
        }

        void Input::MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
        {
            switch(action)
            {
                case GLFW_PRESS:
                    keys[button] = Down;
                    break;
                case GLFW_RELEASE:
                    keys[button] = Up;
                    break;
            }
        }

        bool Input::GetKey(KeyButtonStatus status, KeyCode key)
        {
            return keys[key] == status;
        }

        int Input::GetMouseScrollDelta()
        {
            return mouse.scrollDelta;
        }

        Vector2 Input::GetMousePosition()
        {
            return mouse.position;
        }

        Vector2 Input::GetMousePositionDelta()
        {
            return mouse.positionDelta;
        }

        Vector3 Input::GetMovementVector()
        {
            Vector3 axis(0, 0, 0);

            if(GetKey(Pressed, KeyCode::W) || GetKey(Pressed, KeyCode::UpArrow))
                axis.y += 1;
            if(GetKey(Pressed, KeyCode::S) || GetKey(Pressed, KeyCode::DownArrow))
                axis.y -= 1;

            if(GetKey(Pressed, KeyCode::D) || GetKey(Pressed, KeyCode::RightArrow))
                axis.x += 1;
            if(GetKey(Pressed, KeyCode::A) || GetKey(Pressed, KeyCode::LeftArrow))
                axis.x -= 1;

            if(GetKey(Pressed, KeyCode::LeftControl) || GetKey(Pressed, KeyCode::RightControl))
                axis.z -= 1;
            if(GetKey(Pressed, KeyCode::LeftShift) || GetKey(Pressed, KeyCode::RightShift))
                axis.z += 1;

            return axis;
        }

#pragma endregion

    }
}
