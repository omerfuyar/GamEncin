#pragma once
#include <array>
#include <cmath>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using std::array;
using std::vector;
using std::string;
using std::unordered_map;
using std::map;

namespace GamEncin
{
    enum Layer
    {
        Default, IgnoreRay, UI, Player, Layer1, Layer2, Layer3
    };

    struct MathYaman
    {
    public:
        //Euler, PI, Earth's gravity, Gravitational constant
        static const float E, PI, GRAVITY, G;
    };

    class Vector2
    {
    private:
        static const Vector2 zero, one, up, right, down, left;

    public:

        float x, y;

        Vector2(float x = 0, float y = 0) : x(x), y(y) {};

        ~Vector2() = default;

#pragma region Operators

        Vector2 operator + (const Vector2& other)
        {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2 operator += (const Vector2& other)
        {
            x += other.x;
            y += other.y;
            return Vector2(x, y);
        }

        Vector2 operator - (const Vector2& other)
        {
            return Vector2(x - other.x, y - other.y);
        }

        Vector2 operator -= (const Vector2& other)
        {
            x -= other.x;
            y -= other.y;
            return Vector2(x, y);
        }

        Vector2 operator * (const float other)
        {
            return Vector2(x * other, y * other);
        }

        Vector2 operator *= (const float other)
        {
            x *= other;
            y *= other;
            return Vector2(x, y);
        }

        Vector2 operator / (const float other)
        {
            return Vector2(x / other, y / other);
        }

        Vector2 operator /= (const float other)
        {
            x /= other;
            y /= other;
            return Vector2(x, y);
        }

#pragma endregion

        float GetMagnitude()
        {
            return sqrt(x * x + y * y);
        }

        //Scales and returns the Vector2 magnitude of 1
        Vector2 Normalize()
        {
            float magnitude = GetMagnitude();
            x /= magnitude;
            y /= magnitude;
            return Vector2(x, y);
        }

        //Returns the normalized version of the Vector2
        Vector2 Normalized()
        {
            Vector2 result = Vector2(x, y);
            result.Normalize();
            return result;
        }

        float Set(float newX, float newY)
        {
            x = newX;
            y = newY;
        }

        //Returns a Vector2(0, 0)
        static const Vector2 Zero() { return zero; }
        //Returns a Vector2(1, 1)
        static const Vector2 One() { return one; }
        //Returns a Vector2(0, 1)
        static const Vector2 Up() { return up; }
        //Returns a Vector2(1, 0)
        static const Vector2 Right() { return right; }
        //Returns a Vector2(0, -1)
        static const Vector2 Down() { return down; }
        //Returns a Vector2(-1, 0)
        static const Vector2 Left() { return left; }
    };

    class Vector3
    {
    private:
        static const Vector3 zero, one, up, right, down, left, forward, backward;

    public:
        float x, y, z;

        Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        ~Vector3() = default;

#pragma region Operators

        Vector3 operator + (const Vector3& other)
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 operator += (const Vector3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return Vector3(x, y, z);
        }

        Vector3 operator - (const Vector3& other)
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 operator -= (const Vector3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return Vector3(x, y, z);
        }

        Vector3 operator * (const float other)
        {
            return Vector3(x * other, y * other, z * other);
        }

        Vector3 operator *= (const float other)
        {
            x *= other;
            y *= other;
            z *= other;
            return Vector3(x, y, z);
        }

        Vector3 operator / (const float other)
        {
            return Vector3(x / other, y / other, z / other);
        }

        Vector3 operator /= (const float other)
        {
            x /= other;
            y /= other;
            z /= other;
            return Vector3(x, y, z);
        }

#pragma endregion

        float GetMagnitude()
        {
            return sqrt(x * x + y * y + z * z);
        }

        //Scales and returns the Vector3 magnitude of 1
        Vector3 Normalize()
        {
            float magnitude = GetMagnitude();
            x /= magnitude;
            y /= magnitude;
            z /= magnitude;
            return Vector3(x, y, z);
        }

        //Returns the normalized version of the Vector2
        Vector3 Normalized()
        {
            Vector3 result = Vector3(x, y, z);
            result.Normalize();
            return result;
        }

        float Set(float newX, float newY, float newZ)
        {
            x = newX;
            y = newY;
            z = newZ;
        }

        static float* VerticesVectorToFloatArr(vector<Vector3> vertices);

        //Returns a Vector3(0, 0, 0)
        static const Vector3 Zero() { return zero; }
        //Returns a Vector3(1, 1, 1)
        static const Vector3 One() { return one; }
        //Returns a Vector3(0, 1, 0)
        static const Vector3 Up() { return up; }
        //Returns a Vector3(1, 0, 0)
        static const Vector3 Right() { return right; }
        //Returns a Vector3(0, -1, 0)
        static const Vector3 Down() { return down; }
        //Returns a Vector3(-1, 0, 0)
        static const Vector3 Left() { return left; }
        //Returns a Vector3(0, 0, 1)
        static const Vector3 Forward() { return forward; }
        //Returns a Vector3(0, 0, -1)
        static const Vector3 Backward() { return backward; }
    };

    class Vector4
    {
    private:
        static const Vector4 zero, one;

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
        static const Vector4 Zero() { return zero; }
        //Returns a Vector4(1, 1, 1, 1)
        static const Vector4 One() { return one; }
    };
}
