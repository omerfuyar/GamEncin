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

        GamePadButtonCode gamePadButtonArr[] = {
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

#pragma region GamePad

        void GamePad::Initialize(GLFWwindow* window)
        {
            name = glfwGetGamepadName(ID);

            for(GamePadButtonCode code : gamePadButtonArr)
            {
                buttons[code] = Released;
            }
        }

        void GamePad::Update(GLFWwindow* window)
        {
            if(!glfwGetGamepadState(ID, &state))
            {
                Application::Stop(IODeviceErr);
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
        map<int, GamePad*> Input::gamePads;

        void Input::GamePadCallBack(int joyStickID, int event)
        {
            if(!glfwJoystickIsGamepad(joyStickID)) return; //TODO throw warning or info message, only supports gamepads

            if(event == GLFW_CONNECTED)
            {
                GamePad* gamePad = new GamePad(joyStickID);
                gamePad->Initialize(window);
                printf("Gamepad connected: %s\n", gamePad->name.c_str());
                gamePads[joyStickID] = gamePad;
            }

            else if(event == GLFW_DISCONNECTED)
            {
                printf("Gamepad disconnected: %s\n", gamePads[joyStickID]->name.c_str());
                delete gamePads[joyStickID];
                gamePads.erase(joyStickID);
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

            glfwSetJoystickCallback(GamePadCallBack);

            for(int i = 0; i < 16; i++)
            {
                if(glfwJoystickIsGamepad(i)) //0-15
                {
                    GamePad* gamePad = new GamePad(i);
                    gamePad->Initialize(window);
                    printf("Gamepad detected: %s\n", glfwGetGamepadName(i));
                    gamePads[i] = gamePad;
                }
                else
                    if(!glfwJoystickIsGamepad(i)) continue; //TODO throw warning or info message, only supports gamepads
            }

            mouse.Initialize(window);
            keyboard.Initialize(window);
        }

        void Input::UpdateInputs()
        {
            //resetting values
            mouse.Update(window);
            keyboard.Update(window);
            for(auto& gamePad : gamePads)
                gamePad.second->Update(window);

            glfwPollEvents();
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

        bool Input::GetGamePadButton(int gamePadID, KeyButtonStatus status, GamePadButtonCode button)
        {
            if(gamePads.find(gamePadID) == gamePads.end())
            {
                //TODO throw warning or info message, gamepad not found
                return false;
            }
            return gamePads[gamePadID]->buttons[button] == status;
        }

        float Input::GetGamePadLeftTrigger(int gamePadID)
        {
            if(gamePads.find(gamePadID) == gamePads.end())
            {
                //TODO throw warning or info message, gamepad not found
                return 0;
            }
            return gamePads[gamePadID]->leftTrigger;
        }

        float Input::GetGamePadRightTrigger(int gamePadID)
        {
            if(gamePads.find(gamePadID) == gamePads.end())
            {
                //TODO throw warning or info message, gamepad not found
                return 0;
            }
            return gamePads[gamePadID]->rightTrigger;
        }

        Vector2 Input::GetGamePadLeftStick(int gamePadID)
        {
            if(gamePads.find(gamePadID) == gamePads.end())
            {
                //TODO throw warning or info message, gamepad not found
                return Vector2::Zero();
            }
            return gamePads[gamePadID]->leftStick;
        }

        Vector2 Input::GetGamePadRightStick(int gamePadID)
        {
            if(gamePads.find(gamePadID) == gamePads.end())
            {
                //TODO throw warning or info message, gamepad not found
                return Vector2::Zero();
            }
            return gamePads[gamePadID]->rightStick;
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
