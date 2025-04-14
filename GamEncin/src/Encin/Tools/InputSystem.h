#pragma once
#include "Encin/Tools/MathYaman.h"

#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>
#include <windows.h>

namespace GamEncin
{
    namespace InputSystem
    {
        //TODO be careful while adding new inputs, add them also in InputSystem.cpp arrays. they should be identical !!!

        enum KeyCode
        {
            A = GLFW_KEY_A,
            B = GLFW_KEY_B,
            C = GLFW_KEY_C,
            D = GLFW_KEY_D,
            E = GLFW_KEY_E,
            F = GLFW_KEY_F,
            G = GLFW_KEY_G,
            H = GLFW_KEY_H,
            I = GLFW_KEY_I,
            J = GLFW_KEY_J,
            K = GLFW_KEY_K,
            L = GLFW_KEY_L,
            M = GLFW_KEY_M,
            N = GLFW_KEY_N,
            O = GLFW_KEY_O,
            P = GLFW_KEY_P,
            Q = GLFW_KEY_Q,
            R = GLFW_KEY_R,
            S = GLFW_KEY_S,
            T = GLFW_KEY_T,
            U = GLFW_KEY_U,
            V = GLFW_KEY_V,
            W = GLFW_KEY_W,
            X = GLFW_KEY_X,
            Y = GLFW_KEY_Y,
            Z = GLFW_KEY_Z,

            Num0 = GLFW_KEY_0,
            Num1 = GLFW_KEY_1,
            Num2 = GLFW_KEY_2,
            Num3 = GLFW_KEY_3,
            Num4 = GLFW_KEY_4,
            Num5 = GLFW_KEY_5,
            Num6 = GLFW_KEY_6,
            Num7 = GLFW_KEY_7,
            Num8 = GLFW_KEY_8,
            Num9 = GLFW_KEY_9,

            Space = GLFW_KEY_SPACE,
            Enter = GLFW_KEY_ENTER,
            BackSpace = GLFW_KEY_BACKSPACE,
            Escape = GLFW_KEY_ESCAPE,
            Tab = GLFW_KEY_TAB,
            LeftShift = GLFW_KEY_LEFT_SHIFT,
            RightShift = GLFW_KEY_RIGHT_SHIFT,
            LeftControl = GLFW_KEY_LEFT_CONTROL,
            RightControl = GLFW_KEY_RIGHT_CONTROL,
            LeftAlt = GLFW_KEY_LEFT_ALT,
            RightAlt = GLFW_KEY_RIGHT_ALT,
            CapsLock = GLFW_KEY_CAPS_LOCK,
            UpArrow = GLFW_KEY_UP,
            DownArrow = GLFW_KEY_DOWN,
            LeftArrow = GLFW_KEY_LEFT,
            RightArrow = GLFW_KEY_RIGHT,

            F1 = GLFW_KEY_F1,
            F2 = GLFW_KEY_F2,
            F3 = GLFW_KEY_F3,
            F4 = GLFW_KEY_F4,
            F5 = GLFW_KEY_F5,
            F6 = GLFW_KEY_F6,
            F7 = GLFW_KEY_F7,
            F8 = GLFW_KEY_F8,
            F9 = GLFW_KEY_F9,
            F10 = GLFW_KEY_F10,
            F11 = GLFW_KEY_F11,
            F12 = GLFW_KEY_F12,
        };

        enum MouseButtonCode
        {
            Left = GLFW_MOUSE_BUTTON_LEFT,
            Right = GLFW_MOUSE_BUTTON_RIGHT,
            Middle = GLFW_MOUSE_BUTTON_MIDDLE,

            Fn1 = GLFW_MOUSE_BUTTON_4,
            Fn2 = GLFW_MOUSE_BUTTON_5,
            Fn3 = GLFW_MOUSE_BUTTON_6,
            Fn4 = GLFW_MOUSE_BUTTON_7,
            Fn5 = GLFW_MOUSE_BUTTON_8
        };

        enum GamepadButtonCode
        {
            A_Cross = GLFW_GAMEPAD_BUTTON_A,
            B_Circle = GLFW_GAMEPAD_BUTTON_B,
            X_Square = GLFW_GAMEPAD_BUTTON_X,
            Y_Triangle = GLFW_GAMEPAD_BUTTON_Y,

            L1 = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
            R1 = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
            L3 = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
            R3 = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,

            Back = GLFW_GAMEPAD_BUTTON_BACK,
            Start = GLFW_GAMEPAD_BUTTON_START,
            Guide = GLFW_GAMEPAD_BUTTON_GUIDE,

            DPadUp = GLFW_GAMEPAD_BUTTON_DPAD_UP,
            DPadRight = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
            DPadDown = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
            DPadLeft = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
        };

        enum KeyButtonStatus
        {
            Release, Press, Down, Up
        };

        //shouldn't be used by the user
        struct Mouse
        {
        public:
            //Should use MouseButtonCode enum as key
            static unordered_map<int, KeyButtonStatus> buttons;
            static Vector2 position, positionDelta;
            static float scrollDelta;

            static void Initialize(GLFWwindow* window);
            static void Update(GLFWwindow* window);
            static void MouseScrollCallBack(GLFWwindow* window, double offsetX, double offsetY);
            static void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
        };

        //shouldn't be used by the user
        struct KeyBoard
        {
        public:
            //Should use KeyCode enum as key
            static unordered_map<int, KeyButtonStatus> keys;

            static void Initialize(GLFWwindow* window);
            static void Update(GLFWwindow* window);
            static void KeyCallBack(GLFWwindow* window, int key, int scanCode, int action, int mods);
        };

        //shouldn't be used by the user
        struct Gamepad
        {
        public:
            GLFWgamepadstate state = GLFWgamepadstate();
            string name;
            int id;

            //Should use GamepadButtonCode enum as key
            map<int, KeyButtonStatus> buttons;
            float leftTrigger = 0.0,
                rightTrigger = 0.0;
            Vector2 leftStick = Vector2::Zero(),
                rightStick = Vector2::Zero();

            Gamepad(int id);
            void Initialize(GLFWwindow* window);
            void Update(GLFWwindow* window);
        };

        class Input
        {
        private:
            static GLFWwindow* window;
            static Mouse mouse;
            static KeyBoard keyboard;
            static unordered_map<int, Gamepad*> gamepads;

            Input() = delete;
            Input(const Input&) = delete;
            void operator = (const Input&) = delete;

            static void GamepadCallBack(int gamepadId, int event);

        public:
            //shouldn't be used by the user
            static void Initialize(GLFWwindow* window);
            //shouldn't be used by the user
            static void UpdateInputs();
            //shouldn't be used by the user
            static void ConnectGamepad(int gamepadId);
            //shouldn't be used by the user
            static void DisconnectGamepad(int gamepadId);

            /// <summary>
            /// Gets keyboard key inputs from user
            /// </summary>
            /// <param name="status:">Status to get from key</param>
            /// <param name="key:">Key ID with KeyCode enum</param>
            /// <returns>True if the key is in that status</returns>
            static bool GetKey(KeyButtonStatus status, KeyCode key);

            /// <summary>
            /// Gets mouse button inputs from user
            /// </summary>
            /// <param name="status:">Status to get from key</param>
            /// <param name="button:">Button ID with MouseButtonCode enum</param>
            /// <returns>True if the button is in that status</returns>
            static bool GetMouseButton(KeyButtonStatus status, MouseButtonCode button);
            // Gets the scroll delta, change of scroll in last frame, of the mouse. Can be either 0, 1 or -1
            static float GetMouseScrollDelta();
            // Returns the Vector2 which represents mouse position in pixels. Top left corner of the window is the origin
            static Vector2 GetMousePosition();
            // Returns the position delta, change of position in last frame, of the mouse.
            static Vector2 GetMousePositionDelta();

            /// <summary>
            /// Gets gamepad button inputs from user
            /// </summary>
            /// <param name="status:">Status to get from key</param>
            /// <param name="button:">Button ID with GamepadButtonCode enum</param>
            /// <returns>True if the button is in that status</returns>
            static bool GetGamepadButton(int gamepadId, KeyButtonStatus status, GamepadButtonCode button);
            //Returns the float value of the left trigger of given gamepad. 0 is not pressed, 1 is fully pressed
            static float GetGamepadLeftTrigger(int gamepadId);
            //Returns the float value of the right trigger of given gamepad. 0 is not pressed, 1 is fully pressed
            static float GetGamepadRightTrigger(int gamepadId);
            //Returns the Vector2 value of the left stick of given gamepad.
            static Vector2 GetGamepadLeftStick(int gamepadId);
            //Returns the Vector2 of the right stick of given gamepad.
            static Vector2 GetGamepadRightStick(int gamepadId);
            //Returns true if the gamepad is connected
            static bool IsGamepadConnected(int gamepadId);

            /// <summary>
            /// Gets the shortcut inputs for WASD and arrow + Control and Shift / Ctrl keys in Vector3.
            /// </summary>
            /// <returns>The movement input of the user</returns>
            static Vector3 GetMovementVector();

            /// <summary>
            /// Gets the file contents of the file
            /// </summary>
            /// <param name="fileName:">Direction and name of the intended file</param>
            /// <returns>A string that contains all file data</returns>
            static string GetFileContents(const char* fileName);

            static string GetExeFilePath();
        };
    }
}

using namespace GamEncin::InputSystem;
