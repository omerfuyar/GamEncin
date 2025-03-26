#pragma once
#include "GraphicLibraries.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using std::vector;
using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::map;
using glm::vec3;
using glm::mat4;

namespace GamEncin
{
    namespace ToolKit
    {
        enum Layer
        {
            Default, IgnoreRay, UI, Player, Layer0, Layer1, Layer2, Layer3, Layer4, Layer5, Layer6, Layer7, Layer8, Layer9
        };

        enum EndType
        {
            Safe, Warning, UnkErr, GLFWErr, GLADErr, ShaderCompilationErr, ShaderLinkingErr, ObjCouldNotFoundErr, TypeMismachErr, IOErr, ProgramRunningErr
        };

        string getFileContents(const char* fileName);
    }

    namespace MathYaman
    {
        //Euler, PI, Earth's gravity, Gravitational constant
        const float E = 2.7182817F,
            PI = 3.1415927F,
            GRAVITY = 9.80665F,
            G = 6.67F / 1e11F;

#pragma region Functions

        //Returns the absolute value of the float value
        inline float Abs(float value)
        {
            return value < 0 ? -value : value;
        }

        //Returns the root of the float value
        inline float Root(float value, float root)
        {
            return pow(value, 1 / root);
        }

        //Return square root of the float value
        inline float Sqrt(float value)
        {
            return sqrt(value);
        }

        //Returns the cube root of the float value
        inline float Cbrt(float value)
        {
            return cbrt(value);
        }

        //Returns the power of the base to the exponent
        inline float Power(float base, float exponent)
        {
            return pow(base, exponent);
        }

        //Returns the logarithm of the value to the base
        inline float Log(float value, float base)
        {
            return log(value) / log(base);
        }

        //Returns the logarithm of the value to the base 10
        inline float Log10(float value)
        {
            return log10(value);
        }

        //Returns the natural logarithm of the value
        inline float LogE(float value)
        {
            return log(value);
        }

        //Converts the radian value to degree
        inline float Rad2Deg(float radian)
        {
            return radian * 180 / PI;
        }

        //Converts the degree value to radians
        inline float Deg2Rad(float degree)
        {
            return degree * PI / 180;
        }

        //Returns the sine of the radian value
        inline float SinRad(float radian)
        {
            return sin(radian);
        }

        //Returns the sine of the degree value
        inline float SinDeg(float degree)
        {
            return sin(Deg2Rad(degree));
        }

        //Returns the cosine of the radian value
        inline float CosRad(float radian)
        {
            return cos(radian);
        }

        //Returns the cosine of the degree value
        inline float CosDeg(float degree)
        {
            return cos(Deg2Rad(degree));
        }

        //Returns the tangent of the radian value
        inline float TanRad(float radian)
        {
            return tan(radian);
        }

        //Returns the tangent of the degree value
        inline float TanDeg(float degree)
        {
            return tan(Deg2Rad(degree));
        }

        //Returns the cotangent of the radian value
        inline float CotRad(float radian)
        {
            return 1 / TanRad(radian);
        }

        //Returns the cotangent of the degree value
        inline float CotDeg(float degree)
        {
            return 1 / TanDeg(degree);
        }

        //Returns the smaller value between two float values
        inline float Min(float a, float b)
        {
            return a < b ? a : b;
        }

        //Returns the bigger value between two float values
        inline float Max(float a, float b)
        {
            return a > b ? a : b;
        }

        //Clamps the value between min and max values
        inline float Clamp(float value, float min, float max)
        {
            return value < min ? min : value > max ? max : value;
        }

        //Linear Interpolation between two float values, t is between 0 and 1
        inline float Lerp(float a, float b, float t)
        {
            return a + (b - a) * t;
        }

        // Returns a random float value between inclusive min and max values
        inline float RandomRangeFloat(float min, float max)
        {
            return min + (float) rand() / (float) (RAND_MAX / (max - min));
        }

        // Returns a random integer value between inclusive min and max values
        inline int RandomRangeInteger(int min, int max)
        {
            return min + (int) (rand() % (max - min + 1));
        }

#pragma endregion

#pragma region Vectors

        struct Vector2
        {
        public:

            float x, y;

            Vector2(float x = 0, float y = 0) : x(x), y(y) {};
            Vector2(const glm::vec2& vec) : x(vec.x), y(vec.y) {};
            ~Vector2() = default;

#pragma region Operators

            inline Vector2 operator + (const Vector2& other)
            {
                return Vector2(x + other.x, y + other.y);
            }

            inline Vector2 operator += (const Vector2& other)
            {
                x += other.x;
                y += other.y;
                return Vector2(x, y);
            }

            inline Vector2 operator - (const Vector2& other)
            {
                return Vector2(x - other.x, y - other.y);
            }

            inline Vector2 operator -= (const Vector2& other)
            {
                x -= other.x;
                y -= other.y;
                return Vector2(x, y);
            }

            inline Vector2 operator *= (const Vector2& other)
            {
                x *= other.x;
                y *= other.y;
                return Vector2(x, y);
            }

            inline Vector2 operator * (const float other)
            {
                return Vector2(x * other, y * other);
            }

            inline Vector2 operator *= (const float other)
            {
                x *= other;
                y *= other;
                return Vector2(x, y);
            }

            inline Vector2 operator / (const float other)
            {
                return Vector2(x / other, y / other);
            }

            inline Vector2 operator /= (const float other)
            {
                x /= other;
                y /= other;
                return Vector2(x, y);
            }

            inline bool operator == (const Vector2& other)
            {
                return x == other.x && y == other.y;
            }

            inline bool operator != (const Vector2& other)
            {
                return x != other.x || y != other.y;
            }

#pragma endregion

#pragma region Functions

            //Returns the magnitude of the Vector2
            inline float GetMagnitude()
            {
                return Sqrt(x * x + y * y);
            }

            //Scales the Vector2 to magnitude of 1 and returns it
            inline Vector2 Normalize()
            {
                float magnitude = GetMagnitude();
                x /= magnitude;
                y /= magnitude;
                return Vector2(x, y);
            }

            //Returns the normalized version of the Vector2
            inline Vector2 Normalized()
            {
                Vector2 result = Vector2(x, y);
                result.Normalize();
                return result;
            }

            //Returns the glm vec2 versiob of the Vector2
            inline glm::vec2 ToGLMVec2()
            {
                return glm::vec2(x, y);
            }

            //Returns the distance between two Vector2 point
            inline static float Distance(const Vector2 startVec, const Vector2 endVec)
            {
                float num1 = startVec.x - endVec.x;
                float num2 = startVec.y - endVec.y;
                return Sqrt(num1 * num1 + num2 * num2);
            }

            //Linear Interpolation between two Vector2, t is between 0 and 1
            inline static Vector2 Lerp(Vector2 startVec, Vector2 endVec, float t)
            {
                return startVec + (endVec - startVec) * t;
            }

            //Returns a Vector2(0, 0)
            static inline Vector2 Zero() { return Vector2(0, 0); }
            //Returns a Vector2(1, 1)                            
            static inline Vector2 One() { return Vector2(1, 1); }
            //Returns a Vector2(0, 1)                           
            static inline Vector2 Up() { return Vector2(0, 1); }
            //Returns a Vector2(1, 0)                          
            static inline Vector2 Right() { return Vector2(1, 0); }
            //Returns a Vector2(0, -1)
            static inline Vector2 Down() { return Vector2(0, -1); }
            //Returns a Vector2(-1, 0)
            static inline Vector2 Left() { return Vector2(-1, 0); }

#pragma endregion
        };

        struct Vector3
        {
        public:
            float x, y, z;

            Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
            Vector3(const glm::vec3& vec) : x(vec.x), y(vec.y), z(vec.z) {}
            Vector3(const Vector2& vec) : x(vec.x), y(vec.y), z(0) {}
            ~Vector3() = default;

#pragma region Operators

            inline Vector3 operator + (const Vector3& other)
            {
                return Vector3(x + other.x, y + other.y, z + other.z);
            }

            inline Vector3 operator += (const Vector3& other)
            {
                x += other.x;
                y += other.y;
                z += other.z;
                return Vector3(x, y, z);
            }

            inline Vector3 operator - (const Vector3& other)
            {
                return Vector3(x - other.x, y - other.y, z - other.z);
            }

            inline Vector3 operator -= (const Vector3& other)
            {
                x -= other.x;
                y -= other.y;
                z -= other.z;
                return Vector3(x, y, z);
            }

            inline Vector3 operator * (const Vector3& other)
            {
                return Vector3(x * other.x, y * other.y, z * other.z);
            }

            inline Vector3 operator *= (const Vector3& other)
            {
                x *= other.x;
                y *= other.y;
                z *= other.z;
                return Vector3(x, y, z);
            }

            inline Vector3 operator * (const float other)
            {
                return Vector3(x * other, y * other, z * other);
            }

            inline Vector3 operator *= (const float other)
            {
                x *= other;
                y *= other;
                z *= other;
                return Vector3(x, y, z);
            }

            inline Vector3 operator / (const float other)
            {
                return Vector3(x / other, y / other, z / other);
            }

            inline Vector3 operator /= (const float other)
            {
                x /= other;
                y /= other;
                z /= other;
                return Vector3(x, y, z);
            }

            inline bool operator == (const Vector3& other)
            {
                return x == other.x && y == other.y && z == other.z;
            }

            inline bool operator != (const Vector3& other)
            {
                return x != other.x || y != other.y || z != other.z;
            }

#pragma endregion

#pragma region Functions

            //Returns the magnitude of the Vector3
            inline float GetMagnitude()
            {
                return Sqrt(x * x + y * y + z * z);
            }

            //Scales and returns the Vector3 magnitude of 1
            inline Vector3 Normalize()
            {
                float magnitude = GetMagnitude();
                x /= magnitude;
                y /= magnitude;
                z /= magnitude;
                return Vector3(x, y, z);
            }

            //Returns the normalized version of the Vector2
            inline Vector3 Normalized()
            {
                Vector3 result = Vector3(x, y, z);
                result.Normalize();
                return result;
            }

            //Returns the cross product of two Vector3
            inline Vector3 Cross(const Vector3& otherVec) const
            {
                return Vector3(
                    y * otherVec.z - z * otherVec.y,
                    z * otherVec.x - x * otherVec.z,
                    x * otherVec.y - y * otherVec.x
                );
            }

            //Returns the glm vec3 version of the Vector3
            inline glm::vec3 ToGLMvec3()
            {
                return glm::vec3(x, y, z);
            }

            //Returns the distance between two Vector3 point
            inline static float Distance(const Vector3 startVec, const Vector3 endVec)
            {
                float num1 = startVec.x - endVec.x;
                float num2 = startVec.y - endVec.y;
                float num3 = startVec.z - endVec.z;
                return (float) Sqrt(num1 * num1 + num2 * num2 + num3 * num3);
            }

            //Linear Interpolation between two Vector3, t is between 0 and 1
            inline static Vector3 Lerp(Vector3 startVec, Vector3 endVec, float t)
            {
                return startVec + (endVec - startVec) * t;
            }

            //Returns a Vector3(0, 0, 0)
            static inline Vector3 Zero() { return Vector3(0, 0, 0); }
            //Returns a Vector3(1, 1, 1)
            static inline Vector3 One() { return Vector3(1, 1, 1); }
            //Returns a Vector3(0, 1, 0)
            static inline Vector3 Up() { return Vector3(0, 1, 0); }
            //Returns a Vector3(1, 0, 0)
            static inline Vector3 Right() { return Vector3(1, 0, 0); }
            //Returns a Vector3(0, -1, 0)
            static inline Vector3 Down() { return Vector3(0, -1, 0); }
            //Returns a Vector3(-1, 0, 0)
            static inline Vector3 Left() { return Vector3(-1, 0, 0); }
            //Returns a Vector3(0, 0, 1)
            static inline Vector3 Forward() { return Vector3(0, 0, 1); }
            //Returns a Vector3(0, 0, -1)
            static inline Vector3 Backward() { return Vector3(0, 0, -1); }

#pragma endregion
        };

        struct Vector4
        {
        public:
            float x, y, z, w;

            Vector4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}
            Vector4(const glm::vec4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
            Vector4(const Vector2& vec) : x(vec.x), y(vec.y), z(0), w(0) {}
            Vector4(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z), w(0) {}
            ~Vector4() = default;

            //Returns a Vector4(0, 0, 0, 0)
            static inline Vector4 Zero() { return Vector4(0, 0, 0, 0); }
            //Returns a Vector4(1, 1, 1, 1)
            static inline Vector4 One() { return Vector4(1, 1, 1, 1); }
        };

#pragma endregion

    }

    using namespace MathYaman;

    namespace InputSystem
    {
        //TODO be careful while adding new inputs, add them also in Tools.cpp arrays they should be identical

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
            MouseLeft = GLFW_MOUSE_BUTTON_LEFT,
            MouseRight = GLFW_MOUSE_BUTTON_RIGHT,
            MouseMiddle = GLFW_MOUSE_BUTTON_MIDDLE
        };

        struct Key
        {
            bool isPressed;
            bool isDown;
            bool isUp;
        };

        struct MouseButton
        {
            bool isPressed;
            bool isDown;
            bool isUp;
        };

        class Mouse
        {
        public:
            static Vector2 position, positionDelta;
            static int scrollDelta;
            static map<int, MouseButton> buttons;

            static void Initialize(GLFWwindow* window);
            static void Update(GLFWwindow* window);
            static void MouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);
            static void MouseButtonCallBack(GLFWwindow* window, int button, int action, int mods);
        };

        class Input
        {
        private:
            static GLFWwindow* window;
            static Mouse mouse;
            static map<int, Key> keys;

            Input() = delete;
            Input(const Input&) = delete;
            void operator=(const Input&) = delete;

        public:
            //shouldn't be used by the user
            static void Initialize(GLFWwindow* window);
            //shouldn't be used by the user
            static void UpdateInputs();
            //shouldn't be used by the user
            static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);

            //Returns true if the key is pressed in this frame
            static bool GetKey(KeyCode key);
            //Returns true if the key is pressed down in this frame
            static bool GetKeyDown(KeyCode key);
            //Returns true if the key is released in this frame
            static bool GetKeyUp(KeyCode key);

            //Returns true if the mouse button is pressed in this frame
            static bool GetMouseButton(MouseButtonCode mouseButton);
            //Returns true if the mouse button is pressed down in this frame
            static bool GetMouseButtonDown(MouseButtonCode mouseButton);
            //Returns true if the mouse button is released in this frame
            static bool GetMouseButtonUp(MouseButtonCode mouseButton);
            //Returns the scroll delta of the mouse. Either 0, 1 or -1
            static int GetMouseScrollDelta();
            //Returns the position of the mouse in pixels. Top left corner is the origin
            static Vector2 GetMousePosition();
            //Returns the difference between the current and the last frame's mouse position
            static Vector2 GetMousePositionDelta();

            //Returns the movement input of the user, shortcut for WASD and arrow + Control and Shift keys. 
            static Vector3 GetMovementVector();
        };
    }
}
