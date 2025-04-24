#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/transform.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <map>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

using std::array;
using std::map;
using std::string;
using std::unordered_map;
using std::vector;

typedef glm::mat4 Matrix4;

namespace GamEncin
{
    namespace MathYaman
    {
#pragma region Constants

        // Pi
        extern const double PI;
        // Euler's number
        extern const double E;
        // Earth's gravity
        extern const double GRAVITY;
        // Gravitational constant
        extern const double G;

#pragma endregion

#pragma region Basic Functions

        // Returns the factorial of the number
        int Factorial(int n);

        // Returns true if the value is between min and max values, inclusive
        bool Between(float value, float min, float max);

        // Returns the absolute value of the float
        float Abs(float value);

        // Returns the rounded value of the float
        int Round(float value);

        // Returns the root of the float value
        float Root(float value, float root);

        // Return square root of the float value
        float SquareRoot(float value);

        // Returns the cube root of the float value
        float CubeRoot(float value);

        // Returns the power of the base to the exponent
        float Power(float base, float exponent);

        // Returns the Square of the float value
        float Square(float value);

        // Returns the logarithm of the value to the base
        float Log(float value, float base);

        // Returns the logarithm of the value to the base 10
        float Log10(float value);

        // Returns the natural logarithm of the value
        float LogE(float value);

        // Converts the radian value to degree
        float Rad2Deg(float radian);

        // Converts the degree value to radians
        float Deg2Rad(float degree);

        // Returns the angle between two angles in degrees
        float DegBetweenAngles(float a, float b);

        // Returns the angle between two angles in radians
        float RadBetweenAngles(float a, float b);

        // Returns the sine of the radian value
        float SinRad(float radian);

        // Returns the sine of the degree value
        float SinDeg(float degree);

        // Returns the cosine of the radian value
        float CosRad(float radian);

        // Returns the cosine of the degree value
        float CosDeg(float degree);

        // Returns the tangent of the radian value
        float TanRad(float radian);

        // Returns the tangent of the degree value
        float TanDeg(float degree);

        // Returns the arc tangent in degrees
        float ArcTanDeg(float x);

        // Returns the arc tangent in radians
        float ArcTanRad(float x);

        // Returns the angle that points to the given x and y coordinates in degrees
        float ArcTan2Deg(float y, float x);

        // Returns the angle that points to the given x and y coordinates in radians
        float ArcTan2Rad(float y, float x);

        // Returns the cotangent of the radian value
        float CotRad(float radian);

        // Returns the cotangent of the degree value
        float CotDeg(float degree);

#pragma endregion

#pragma region Vectors

        struct Vector2
        {
        public:

            float x, y;

            Vector2(float x = 0, float y = 0);
            Vector2(const glm::vec2& vec);

            Vector2 operator + (const Vector2& other) const;
            Vector2& operator += (const Vector2& other);
            Vector2 operator - (const Vector2& other) const;
            Vector2& operator -= (const Vector2& other);
            Vector2 operator * (const float other) const;
            Vector2& operator *= (const float other);
            Vector2 operator * (const Vector2& other) const;
            Vector2& operator *= (const Vector2& other);
            Vector2 operator / (const float other) const;
            Vector2& operator /= (const float other);
            bool operator == (const Vector2& other);
            bool operator != (const Vector2& other);

            // Returns the magnitude of the Vector2
            float GetMagnitude() const;
            // Scales the Vector2 to magnitude of 1 and returns it
            Vector2& Normalize();
            // Returns the normalized version of the Vector2
            Vector2 Normalized() const;
            // Returns the glm vec2 version of the Vector2
            glm::vec2 ToGLMVec2();

            //Returns a Vector2(0, 0)
            static Vector2 Zero();
            //Returns a Vector2(1, 1)                            
            static Vector2 One();
            //Returns a Vector2(0, 1)                           
            static Vector2 Up();
            //Returns a Vector2(1, 0)                          
            static Vector2 Right();
            //Returns a Vector2(0, -1)
            static Vector2 Down();
            //Returns a Vector2(-1, 0)
            static Vector2 Left();
        };

        struct Vector3
        {
        public:
            float x, y, z;

            Vector3(float x = 0, float y = 0, float z = 0);
            Vector3(const glm::vec3& vec);
            Vector3(const Vector2& vec);
            Vector3(const Vector2& vec, float z);

            Vector3 operator + (const Vector3& other) const;
            Vector3& operator += (const Vector3& other);
            Vector3 operator - (const Vector3& other) const;
            Vector3& operator -= (const Vector3& other);
            Vector3 operator * (const float other) const;
            Vector3& operator *= (const float other);
            Vector3 operator * (const Vector3& other) const;
            Vector3& operator *= (const Vector3& other);
            Vector3 operator / (const float other) const;
            Vector3& operator /= (const float other);
            Vector3 operator * (const Matrix4 other) const;
            bool operator==(const Vector3& other);
            bool operator!=(const Vector3& other);

            // Returns the magnitude of the Vector3
            float GetMagnitude() const;

            // Scales and returns the Vector3 magnitude of 1
            Vector3& Normalize();

            // Returns the normalized version of the Vector3
            Vector3 const Normalized() const;

            /// <summary>
            /// Returns the cross product of two Vector3
            /// </summary>
            /// <param name="otherVec: ">Vector to take cross</param>
            Vector3 const Cross(const Vector3& otherVec);

            // Returns the dot product of two Vector3
            float const Dot(const Vector3& otherVec);

            // Returns the glm vec3 version of the Vector3
            glm::vec3 const ToGLMvec3() const;

            // Returns a Vector3(0, 0, 0)
            static Vector3 Zero();
            // Returns a Vector3(1, 1, 1)
            static Vector3 One();
            // Returns a Vector3(0, 1, 0)
            static Vector3 Up();
            // Returns a Vector3(1, 0, 0)
            static Vector3 Right();
            // Returns a Vector3(0, -1, 0)
            static Vector3 Down();
            // Returns a Vector3(-1, 0, 0)
            static Vector3 Left();
            // Returns a Vector3(0, 0, 1)
            static Vector3 Forward();
            // Returns a Vector3(0, 0, -1)
            static Vector3 Backward();
        };

        struct Vector4
        {
        public:
            float x, y, z, w;

            Vector4(float x = 0, float y = 0, float z = 0, float w = 1);
            Vector4(const glm::vec4& vec);
            Vector4(const Vector2& vec);
            Vector4(const Vector3& vec);

            // Returns a Vector4(0, 0, 0, 0)
            static Vector4 Zero();
            // Returns a Vector4(1, 1, 1, 1)
            static Vector4 One();
        };

        struct Vector2Int
        {
            int x, y;

            Vector2Int(int x = 0, int y = 0);
            Vector2Int(const glm::ivec2& vec);
            Vector2Int(const Vector2& vec);

            // Operators
            Vector2Int operator + (const Vector2Int& other) const;
            Vector2Int& operator += (const Vector2Int& other);
            Vector2Int operator - (const Vector2Int& other) const;
            Vector2Int& operator -= (const Vector2Int& other);
            Vector2Int operator * (int other) const;
            Vector2Int& operator *= (int other);
            Vector2Int operator * (const Vector2Int& other) const;
            Vector2Int& operator *= (const Vector2Int& other);
            Vector2Int operator / (int other) const;
            Vector2Int& operator /= (int other);
            bool operator == (const Vector2Int& other);
            bool operator != (const Vector2Int& other);

            // Returns the magnitude of the Vector2Int
            float GetMagnitude() const;
            // Scales the Vector2Int to 8 direction
            Vector2Int& EightDirection();
            // Returns the 8 direction version of the Vector2Int
            Vector2Int EightDirectioned() const;
            // Returns the normalized version of the Vector2Int
            Vector2 Normalized() const;

            // Returns a Vector2Int(0, 0)
            static Vector2Int Zero();
            // Returns a Vector2Int(1, 1)
            static Vector2Int One();
            // Returns a Vector2Int(0, 1)
            static Vector2Int Up();
            // Returns a Vector2Int(1, 0)
            static Vector2Int Right();
            // Returns a Vector2Int(0, -1)
            static Vector2Int Down();
            // Returns a Vector2Int(-1, 0)
            static Vector2Int Left();
        };

#pragma endregion

#pragma region Advenced Functions

        /// <summary>
        /// Generates a random float
        /// </summary>
        /// <param name="min:">Minimum border, inclusive</param>
        /// <param name="max:">Maximum border, inclusive</param>
        /// <returns>A random float value between min and max values</returns>
        float RandomRangeFloat(float min, float max);

        /// <summary>
        /// Generates a random integer
        /// </summary>
        /// <param name="min:">Minimum border, inclusive</param>
        /// <param name="max:">Maximum border, inclusive</param>
        /// <returns>A random integer value between min and max values</returns>
        int RandomRangeInteger(int min, int max);

        /// <summary>
        /// Generates a random Vector2
        /// </summary>
        /// <returns>A random point in unit circle</returns>
        Vector2 RandomVector2();

        /// <summary>
        /// Generates a random Vector3
        /// </summary>
        /// <returns>A random point in unit sphere</returns>
        Vector3 RandomVector3();

        /// <summary>
        /// Generates a random Vector2 direction with magnitude of 1
        /// </summary>
        /// <returns>A normalized random Vector2</returns>
        Vector2 RandomVector2Direction();

        /// <summary>
        /// Generates a random Vector3 direction with magnitude of 1
        /// </summary>
        /// <returns>A normalized random Vector3</returns>
        Vector3 RandomVector3Direction();

        // Returns the smaller value between two float values
        float Min(float a, float b);

        // Returns the vector with smaller magnitude between two Vector2
        Vector2 Min(Vector2 a, Vector2 b);

        // Returns the vector with smaller magnitude between two Vector3
        Vector3 Min(Vector3 a, Vector3 b);

        // Returns the greater value between two float values
        float Max(float a, float b);

        // Returns the vector with greater magnitude between two Vector2
        Vector2 Max(Vector2 a, Vector2 b);

        // Returns the vector with greater magnitude between two Vector3
        Vector3 Max(Vector3 a, Vector3 b);

        /// <summary>
        /// Clamps the value between min and max values
        /// </summary>
        /// <param name="value:">Value to clamp</param>
        /// <param name="min:">Min value</param>
        /// <param name="max:">Max value</param>
        /// <returns>Min, max or inbetween float</returns>
        float Clamp(float value, float min, float max);

        /// <summary>
        /// Sets the value to min if it is smaller than min
        /// </summary>
        /// <param name="value">Value to clamp</param>
        /// <param name="min">Min value</param>
        /// <returns></returns>
        float ClampMin(float value, float min);

        /// <summary>
        /// Sets the value to max if it is greater than max
        /// </summary>
        /// <param name="value">Value to clamp</param>
        /// <param name="max">Max value</param>
        /// <returns></returns>
        float ClampMax(float value, float max);

        /// <summary>
        /// Clamps the Vector2 magnitude between 0 and max value
        /// </summary>
        /// <param name="value:">Vector to clamp</param>
        /// <param name="max:">Max magnitude</param>
        /// <returns>Original vector or a Vector2 with max magnitude</returns>
        Vector2 ClampVectorMagnitude(Vector2 vector, float max);

        /// <summary>
        /// Clamps the Vector3 magnitude between 0 and max value
        /// </summary>
        /// <param name="value:">Vector to restrict</param>
        /// <param name="max:">Max magnitude</param>
        /// <returns>Original vector or a Vector3 with max magnitude</returns>
        Vector3 ClampVectorMagnitude(Vector3 vector, float max);

        /// <summary>
        /// Linear Interpolation between two float values
        /// </summary>
        /// <param name="startVal:">Start value</param>
        /// <param name="endVal:">End value</param>
        /// <param name="t:">Between 0 and 1. 0 is startVal, 1 is endVal</param>
        /// <returns>The float value between start and end values depending on t</returns>
        float Lerp(float startVal, float endVal, float t);

        /// <summary>
        /// Linear Interpolation between two Vector2
        /// </summary>
        /// <param name="startVal:">Start vector</param>
        /// <param name="endVal:">End vector</param>
        /// <param name="t:">Between 0 and 1. 0 is startVec, 1 is endVec</param>
        /// <returns>The Vector2 between start and end vectors depending on t</returns>
        Vector2 Lerp(Vector2 startVec, Vector2 endVec, float t);

        /// <summary>
        /// Linear Interpolation between two Vector3
        /// </summary>
        /// <param name="startVal:">Start vector</param>
        /// <param name="endVal:">End vector</param>
        /// <param name="t:">Between 0 and 1. 0 is startVec, 1 is endVec</param>
        /// <returns>The Vector3 between start and end vectors depending on t</returns>
        Vector3 Lerp(Vector3 startVec, Vector3 endVec, float t);

        /// <summary>
        /// Returns the distance between two Vector2 point
        /// </summary>
        /// <param name="startVec:">Start vector</param>
        /// <param name="endVec:">End vector</param>
        /// <returns> </returns>
        float Distance(Vector2 a, Vector2 b);

        /// <summary>
        /// Returns the distance between two Vector3 point
        /// </summary>
        /// <param name="startVec:">Start vector</param>
        /// <param name="endVec:">End vector</param>
        /// <returns></returns>
        float Distance(Vector3 a, Vector3 b);

        /// <summary>
        /// Returns true if string2 is a substring of string1
        /// </summary>
        /// <param name="string1"></param>
        /// <param name="string2"></param>
        /// <returns></returns>
        bool IsSubString(const char* string1, const char* string2);

#pragma endregion
    }
}

using namespace GamEncin::MathYaman;
