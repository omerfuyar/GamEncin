#pragma once
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using std::vector;
using std::string;
using std::shared_ptr;
using std::unique_ptr;

namespace GamEncin
{
    enum Layer
    {
        Default, IgnoreRay, UI, Player, Layer1, Layer2, Layer3
    };

    enum EndType
    {
        Safe, Warning, UnkErr, GLFWErr, GLADErr, ShaderCompilationErr, ShaderLinkingErr, ObjCouldNotFoundErr, TypeMismachErr, IOErr, AppDuplicationErr
    };

    string getFileContents(const char* fileName);

    struct Vector2
    {
    public:

        float x, y;

        Vector2(float x = 0, float y = 0) : x(x), y(y) {};

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

#pragma endregion

#pragma region Functions

        inline float GetMagnitude()
        {
            return sqrt(x * x + y * y);
        }

        //Scales and returns the Vector2 magnitude of 1
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

        inline float Set(float newX, float newY)
        {
            x = newX;
            y = newY;
        }

        //Returns a Vector2(0, 0)
        static const inline Vector2 Zero() { return Vector2(0, 0); }
        //Returns a Vector2(1, 1)                            
        static const inline Vector2 One() { return Vector2(1, 1); }
        //Returns a Vector2(0, 1)                           
        static const inline Vector2 Up() { return Vector2(0, 1); }
        //Returns a Vector2(1, 0)                          
        static const inline Vector2 Right() { return Vector2(1, 0); }
        //Returns a Vector2(0, -1)
        static const inline Vector2 Down() { return Vector2(0, -1); }
        //Returns a Vector2(-1, 0)
        static const inline Vector2 Left() { return Vector2(-1, 0); }

#pragma endregion
    };

    struct Vector3
    {
    public:
        float x, y, z;

        Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

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

#pragma endregion

#pragma region Functions

        inline float GetMagnitude()
        {
            return sqrt(x * x + y * y + z * z);
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

        inline float Set(float newX, float newY, float newZ)
        {
            x = newX;
            y = newY;
            z = newZ;
        }

        //Returns a Vector3(0, 0, 0)
        static const inline Vector3 Zero() { return Vector3(0, 0, 0); }
        //Returns a Vector3(1, 1, 1)
        static const inline Vector3 One() { return Vector3(1, 1, 1); }
        //Returns a Vector3(0, 1, 0)
        static const inline Vector3 Up() { return Vector3(0, 1, 0); }
        //Returns a Vector3(1, 0, 0)
        static const inline Vector3 Right() { return Vector3(1, 0, 0); }
        //Returns a Vector3(0, -1, 0)
        static const inline Vector3 Down() { return Vector3(0, -1, 0); }
        //Returns a Vector3(-1, 0, 0)
        static const inline Vector3 Left() { return Vector3(-1, 0, 0); }
        //Returns a Vector3(0, 0, 1)
        static const inline Vector3 Forward() { return Vector3(0, 0, 1); }
        //Returns a Vector3(0, 0, -1)
        static const inline Vector3 Backward() { return Vector3(0, 0, -1); }

#pragma endregion
    };

    struct Vector4
    {
    public:
        float x, y, z, w;

        Vector4(float x = 0, float y = 0, float z = 0, float w = 0) : x(x), y(y), z(z), w(w) {}

        ~Vector4() = default;

        float Set(float newX, float newY, float newZ, float newW)
        {
            x = newX;
            y = newY;
            z = newZ;
            w = newW;
        }

        //Returns a Vector4(0, 0, 0, 0)
        static const inline Vector4 Zero() { return Vector4(0, 0, 0, 0); }
        //Returns a Vector4(1, 1, 1, 1)
        static const inline Vector4 One() { return Vector4(1, 1, 1, 1); }
    };

    namespace MathYaman
    {
        //Euler, PI, Earth's gravity, Gravitational constant
        static const float E = 2.7182817F,
            PI = 3.1415927F,
            GRAVITY = 9.80665F,
            G = 6.67F / 1e11F;

#pragma region Functions

        inline float Abs(float value)
        {
            return value < 0 ? -value : value;
        }

        inline float Sqrt(float value)
        {
            return sqrt(value);
        }

        inline float Power(float base, float exponent)
        {
            return pow(base, exponent);
        }

        inline float Log(float value, float base)
        {
            return log(value) / log(base);
        }

        inline float Log10(float value)
        {
            return log10(value);
        }

        inline float LogE(float value)
        {
            return log(value);
        }

        inline float Rad2Deg(float radian)
        {
            return radian * 180 / PI;
        }

        inline float Deg2Rad(float degree)
        {
            return degree * PI / 180;
        }

        inline float SinRad(float radian)
        {
            return sin(radian);
        }

        inline float SinDeg(float degree)
        {
            return sin(Deg2Rad(degree));
        }

        inline float CosRad(float radian)
        {
            return cos(radian);
        }

        inline float CosDeg(float degree)
        {
            return cos(Deg2Rad(degree));
        }

        inline float TanRad(float radian)
        {
            return tan(radian);
        }

        inline float Min(float a, float b)
        {
            return a < b ? a : b;
        }

        inline float Max(float a, float b)
        {
            return a > b ? a : b;
        }

        inline float Clamp(float value, float min, float max)
        {
            return value < min ? min : value > max ? max : value;
        }

        inline float Lerp(float a, float b, float t)
        {
            return a + (b - a) * t;
        }

        inline float RandomRangeFloat(float min, float max)
        {
            return min + (float) rand() / (float) (RAND_MAX / (max - min));
        }

        inline int RandomRangeInteger(int min, int max)
        {
            return min + (int) (rand() % (max - min + 1));
        }

#pragma endregion

#pragma region Matrices

        struct Matrix4x4
        {
            float matrix[4][4];
        };

#pragma endregion

    }
}
