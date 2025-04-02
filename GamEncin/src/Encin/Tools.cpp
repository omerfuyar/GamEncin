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
#pragma region Enum Arrays

        //TODO Be careful with the order of the keys and adding new keys, these arrays should be identical with the enum type in Tools.h
        KeyCode keyCodeArr[] = {
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

            Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,

            Space, Enter, BackSpace, Escape, Tab, LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt, CapsLock, UpArrow, DownArrow, LeftArrow, RightArrow,

            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
        };

        MouseButtonCode mouseButtonArr[] = {
            Left, Right, Middle, Fn1, Fn2, Fn3, Fn4, Fn5
        };

        GamepadButtonCode gamepadButtonArr[] = {
            A_Cross, B_Circle, X_Square, Y_Triangle,

            L1, R1, L3, R3,

            Back, Start, Guide,

            DPadUp, DPadRight, DPadDown, DPadLeft
        };

#pragma endregion

#pragma region Mouse

        map<int, KeyButtonStatus> Mouse::buttons;
        Vector2 Mouse::position = Vector2::Zero(),
            Mouse::positionDelta = Vector2::Zero();
        int Mouse::scrollDelta = 0;

        void Mouse::Initialize(GLFWwindow* window)
        {
            glfwSetMouseButtonCallback(window, MouseButtonCallBack);
            glfwSetScrollCallback(window, MouseScrollCallBack);

            for(MouseButtonCode code : mouseButtonArr)
            {
                buttons[code] = Released;
            }
        }

        void Mouse::Update(GLFWwindow* window)
        {
            double x, y;
            Vector2 temp = position;

            glfwGetCursorPos(window, &x, &y);
            position = Vector2(x, y);
            positionDelta = position - temp;

            scrollDelta = 0;
            for(auto& button : buttons)
            {
                switch(button.second)
                {
                    case Up:
                        button.second = Released;
                        break;
                    case Down:
                        button.second = Pressed;
                        break;
                }
            }
        }

        void Mouse::MouseScrollCallBack(GLFWwindow* window, double offtetX, double offsetY)
        {
            scrollDelta = offsetY;
        }

        void Mouse::MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods)
        {
            switch(action)
            {
                case GLFW_PRESS:
                    buttons[button] = Down;
                    break;
                case GLFW_RELEASE:
                    buttons[button] = Up;
                    break;
            }
        }

#pragma endregion

#pragma region KeyBoard

        map<int, KeyButtonStatus> KeyBoard::keys;

        void KeyBoard::Initialize(GLFWwindow* window)
        {
            glfwSetKeyCallback(window, KeyCallBack);

            for(KeyCode code : keyCodeArr)
            {
                keys[code] = Released;
            }
        }

        void KeyBoard::Update(GLFWwindow* window)
        {
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
                }
            }
        }

        void KeyBoard::KeyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mods)
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

#pragma endregion

#pragma region Gamepad

        void Gamepad::Initialize(GLFWwindow* window)
        {
            name = glfwGetGamepadName(ID);

            for(GamepadButtonCode code : gamepadButtonArr)
            {
                buttons[code] = Released;
            }
        }

        void Gamepad::Update(GLFWwindow* window)
        {
            if(!glfwGetGamepadState(ID, &state))
            {
                Application::PrintLog(IODeviceWarn, "Couldn't get gamepad input.");
                return;
            }

            leftTrigger = state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
            rightTrigger = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
            leftStick = ClampVectorMagnitude(Vector2(state.axes[GLFW_GAMEPAD_AXIS_LEFT_X], state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]), 1);
            rightStick = ClampVectorMagnitude(Vector2(state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]), 1);

            for(auto& button : buttons)
            {
                bool isPressed = state.buttons[button.first];
                switch(button.second)
                {
                    case Pressed:
                        button.second = isPressed ? Pressed : Up;
                        break;
                    case Down:
                        button.second = isPressed ? Pressed : Up;
                        break;
                    case Released:
                        button.second = isPressed ? Down : Released;
                        break;
                    case Up:
                        button.second = isPressed ? Down : Released;
                        break;
                }
            }
        }

#pragma endregion

#pragma region Input

        GLFWwindow* Input::window = nullptr;
        Mouse Input::mouse;
        KeyBoard Input::keyboard;
        map<int, Gamepad*> Input::gamepads;

        void Input::GamepadCallBack(int gamepadID, int event)
        {
            if(!glfwJoystickIsGamepad(gamepadID))
            {
                Application::PrintLog(IODeviceWarn, "Joystick couldn't connected. Application supports only gamepads, not joysticks.");
                return;
            }

            else if(event == GLFW_CONNECTED)
            {
                ConnectGamepad(gamepadID);
            }

            else if(event == GLFW_DISCONNECTED)
            {
                DisconnectGamepad(gamepadID);
            }
        }

        void Input::Initialize(GLFWwindow* _window)
        {
            window = _window;

            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_FALSE);
            glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);

            glfwSetJoystickCallback(GamepadCallBack);

            for(int i = 0; i < 16; i++)
                if(IsGamepadConnected(i)) //0-15
                    ConnectGamepad(i);

            mouse.Initialize(window);
            keyboard.Initialize(window);
        }

        void Input::UpdateInputs()
        {
            //resetting values
            mouse.Update(window);
            keyboard.Update(window);
            for(auto& gamepad : gamepads)
                gamepad.second->Update(window);

            glfwPollEvents();
        }

        void Input::ConnectGamepad(int gamepadID)
        {
            Gamepad* gamepad = new Gamepad(gamepadID);
            gamepad->Initialize(window);

            char message[256];
            sprintf_s(message, "Gamepad connected. ID: %d, Name: %s", gamepadID, gamepad->name.c_str());
            Application::PrintLog(IODeviceWarn, message);

            gamepads[gamepadID] = gamepad;
        }

        void Input::DisconnectGamepad(int gamepadID)
        {
            char message[256];
            sprintf_s(message, "Gamepad disconnected. ID: %d, Name: %s", gamepadID, gamepads[gamepadID]->name.c_str());
            Application::PrintLog(IODeviceWarn, message);

            gamepads[gamepadID]->~Gamepad();
            delete gamepads[gamepadID];
            gamepads[gamepadID] = nullptr;
            gamepads.erase(gamepadID);
        }

        bool Input::GetKey(KeyButtonStatus status, KeyCode key)
        {
            return keyboard.keys[key] == status;
        }

        bool Input::GetMouseButton(KeyButtonStatus status, MouseButtonCode button)
        {
            return mouse.buttons[button] == status;
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

        bool Input::GetGamepadButton(int gamepadID, KeyButtonStatus status, GamepadButtonCode button)
        {
            if(!IsGamepadConnected(0))
            {
                Application::PrintLog(IODeviceWarn, "gamepad couldn't find.");
                return false;
            }
            return gamepads[gamepadID]->buttons[button] == status;
        }

        float Input::GetGamepadLeftTrigger(int gamepadID)
        {
            if(!IsGamepadConnected(0))
            {
                Application::PrintLog(IODeviceWarn, "gamepad couldn't find.");
                return 0;
            }
            return gamepads[gamepadID]->leftTrigger;
        }

        float Input::GetGamepadRightTrigger(int gamepadID)
        {
            if(!IsGamepadConnected(0))
            {
                Application::PrintLog(IODeviceWarn, "gamepad couldn't find.");
                return 0;
            }
            return gamepads[gamepadID]->rightTrigger;
        }

        Vector2 Input::GetGamepadLeftStick(int gamepadID)
        {
            if(!IsGamepadConnected(0))
            {
                Application::PrintLog(IODeviceWarn, "gamepad couldn't find.");
                return Vector2::Zero();
            }
            return gamepads[gamepadID]->leftStick;
        }

        Vector2 Input::GetGamepadRightStick(int gamepadID)
        {
            if(!IsGamepadConnected(0))
            {
                Application::PrintLog(IODeviceWarn, "gamepad couldn't find.");
                return Vector2::Zero();
            }
            return gamepads[gamepadID]->rightStick;
        }

        bool Input::IsGamepadConnected(int gamepadID)
        {
            return glfwJoystickIsGamepad(gamepadID);
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
