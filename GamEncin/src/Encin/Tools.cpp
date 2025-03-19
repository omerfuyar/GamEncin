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

    namespace InputSystem
    {
        //TODO Be careful with the order of the keys and adding new keys, these arrays should be identical with the enum type in Tools.h

        KeyCode keyCodeArr[] = {
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
            Space, Enter, BackSpace, Escape, Tab, LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt, CapsLock, UpArrow, DownArrow, LeftArrow, RightArrow,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
        };

        MouseButtonCode mouseButtonArr[] = {
            MouseLeft, MouseRight, MouseMiddle
        };

#pragma region Mouse

        Vector2 Mouse::position = Vector2(0, 0),
            Mouse::positionDelta = Vector2(0, 0);
        int Mouse::scrollDelta = 0;
        map<int, MouseButton> Mouse::buttons;

        void Mouse::Initialize(GLFWwindow* window)
        {
            glfwSetScrollCallback(window, MouseScrollCallBack);
            glfwSetMouseButtonCallback(window, MouseButtonCallBack);

            for(MouseButtonCode code : mouseButtonArr)
            {
                buttons[code] = {false, false, false};
            }
        }

        void Mouse::Update(GLFWwindow* window)
        {
            double x, y;
            Vector2 temp = position;

            glfwGetCursorPos(window, &x, &y);
            position = Vector2(x, y);
            positionDelta = position - temp;

            //resetting the values
            scrollDelta = 0;
            for(auto& button : buttons)
            {
                button.second.isDown = false;
                button.second.isUp = false;
            }

        }

        void Mouse::MouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
        {
            scrollDelta = yoffset;
        }

        void Mouse::MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
        {
            switch(action)
            {
                case GLFW_PRESS:
                    buttons[button].isPressed = true;
                    buttons[button].isDown = true;
                    buttons[button].isUp = false;
                    break;

                case GLFW_RELEASE:
                    buttons[button].isPressed = false;
                    buttons[button].isDown = false;
                    buttons[button].isUp = true;
                    break;

                case GLFW_REPEAT:
                    buttons[button].isPressed = true;
                    buttons[button].isDown = false;
                    buttons[button].isUp = false;
                    break;
            }
        }

#pragma endregion

#pragma region Input

        GLFWwindow* Input::window = nullptr;
        Mouse Input::mouse;
        map<int, Key> Input::keys;

        void Input::Initialize(GLFWwindow* _window)
        {
            window = _window;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

            glfwSetKeyCallback(window, KeyCallBack);

            for(KeyCode code : keyCodeArr)
            {
                keys[code] = {false, false, false};
            }

            mouse.Initialize(window);
        }

        void Input::UpdateInputs()
        {
            for(auto& key : keys)
            {
                key.second.isDown = false;
                key.second.isUp = false;
            }

            mouse.Update(window);

            glfwPollEvents();
        }

        void Input::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            switch(action)
            {
                case GLFW_PRESS:
                    keys[key].isPressed = true;
                    keys[key].isDown = true;
                    keys[key].isUp = false;
                    break;

                case GLFW_RELEASE:
                    keys[key].isPressed = false;
                    keys[key].isDown = false;
                    keys[key].isUp = true;
                    break;

                case GLFW_REPEAT:
                    keys[key].isPressed = true;
                    keys[key].isDown = false;
                    keys[key].isUp = false;
                    break;
            }
        }

        bool Input::GetKey(KeyCode key)
        {
            return keys[key].isPressed;
        }

        bool Input::GetKeyDown(KeyCode key)
        {
            return keys[key].isDown;
        }

        bool Input::GetKeyUp(KeyCode key)
        {
            return keys[key].isUp;
        }

        bool Input::GetMouseButton(MouseButtonCode mouseButton)
        {
            return mouse.buttons[mouseButton].isPressed;
        }

        bool Input::GetMouseButtonDown(MouseButtonCode mouseButton)
        {
            return mouse.buttons[mouseButton].isDown;
        }

        bool Input::GetMouseButtonUp(MouseButtonCode mouseButton)
        {
            return mouse.buttons[mouseButton].isUp;
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

            if(GetKey(KeyCode::W) || GetKey(KeyCode::UpArrow))
                axis.y += 1;
            if(GetKey(KeyCode::S) || GetKey(KeyCode::DownArrow))
                axis.y -= 1;

            if(GetKey(KeyCode::D) || GetKey(KeyCode::RightArrow))
                axis.x += 1;
            if(GetKey(KeyCode::A) || GetKey(KeyCode::LeftArrow))
                axis.x -= 1;

            if(GetKey(KeyCode::LeftControl) || GetKey(KeyCode::RightControl))
                axis.z -= 1;
            if(GetKey(KeyCode::LeftShift) || GetKey(KeyCode::RightShift))
                axis.z += 1;

            return axis;
        }

#pragma endregion

    }
}
