#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/transform.hpp>

#include <algorithm>
#include <array>
#include <cmath>
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

        struct Vector2;
        struct Vector3;
        struct Vector4;
        struct Vector2Int;
        struct Vector4Int;

#pragma region Basic Functions

        // Returns the factorial of the number
        int Factorial(int n);

        // Returns true if the value is between min and max values, inclusive
        bool Between(float value, float min, float max);

        // Returns the absolute value of the float
        float Abs(float value);

        // Returns 0.0f if the value is zero, 1.0f if the value is positive, and -1.0f if the value is negative
        float Sign(float value);

        // Returns 0 if the value is zero, 1 if the value is positive, and -1 if the value is negative
        int Sign(int value);

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

        // Returns the square of the vector
        Vector2 Square(Vector2 vec);

        // Returns the square of the vector
        Vector3 Square(Vector3 vec);

        // Returns the cube of the float value
        float Cube(float value);

        // Returns the cube of the vector
        Vector2 Cube(Vector2 vec);

        // Returns the cube of the vector
        Vector3 Cube(Vector3 vec);

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
            Vector2(const glm::vec2 &vec);

            Vector2 operator+(const Vector2 &other);
            Vector2 &operator+=(const Vector2 &other);
            Vector2 operator-(const Vector2 &other);
            Vector2 &operator-=(const Vector2 &other);
            Vector2 operator*(const float other);
            Vector2 &operator*=(const float other);
            Vector2 operator*(const Vector2 &other);
            Vector2 &operator*=(const Vector2 &other);
            Vector2 operator/(const float other);
            Vector2 &operator/=(const float other);
            bool operator==(const Vector2 &other);
            bool operator!=(const Vector2 &other);

            // Returns the magnitude of the Vector2
            float GetMagnitude();
            // Scales the Vector2 to magnitude of 1 and returns it
            Vector2 &Normalize();
            // Returns the normalized version of the Vector2
            Vector2 Normalized();
            // Returns the glm vec2 version of the Vector2
            glm::vec2 ToGLMVec2();

            // Returns a Vector2(0, 0)
            static Vector2 Zero();
            // Returns a Vector2(1, 1)
            static Vector2 One();
            // Returns a Vector2(0, 1)
            static Vector2 Up();
            // Returns a Vector2(1, 0)
            static Vector2 Right();
            // Returns a Vector2(0, -1)
            static Vector2 Down();
            // Returns a Vector2(-1, 0)
            static Vector2 Left();
        };

        struct Vector3
        {
        public:
            float x, y, z;

            Vector3(float x = 0, float y = 0, float z = 0);
            Vector3(const glm::vec3 &vec);
            Vector3(const Vector2 &vec);
            Vector3(const Vector2 &vec, float z);

            Vector3 operator+(const Vector3 &other);
            Vector3 &operator+=(const Vector3 &other);
            Vector3 operator-(const Vector3 &other);
            Vector3 &operator-=(const Vector3 &other);
            Vector3 operator*(const float other);
            Vector3 &operator*=(const float other);
            Vector3 operator*(const Vector3 &other);
            Vector3 &operator*=(const Vector3 &other);
            Vector3 operator/(const float other);
            Vector3 &operator/=(const float other);
            Vector3 operator*(const Matrix4 other);
            bool operator==(const Vector3 &other);
            bool operator!=(const Vector3 &other);
            operator Vector2();

            // Returns the magnitude of the Vector3
            float GetMagnitude();

            // Scales and returns the Vector3 magnitude of 1
            Vector3 &Normalize();

            // Returns the normalized version of the Vector3
            Vector3 Normalized();

            /// <summary>
            /// Returns the cross product of two Vector3
            /// </summary>
            /// <param name="otherVec: ">Vector to take cross</param>
            Vector3 Cross(const Vector3 &otherVec);

            // Returns the dot product of two Vector3
            float Dot(const Vector3 &otherVec);

            // Returns the glm vec3 version of the Vector3
            glm::vec3 ToGLMvec3();

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
            Vector4(const glm::vec4 &vec);
            Vector4(const Vector2 &vec);
            Vector4(const Vector3 &vec);

            operator Vector2();
            operator Vector3();

            // Returns a Vector4(0, 0, 0, 0)
            static Vector4 Zero();
            // Returns a Vector4(1, 1, 1, 1)
            static Vector4 One();
        };

        struct Vector2Int
        {
            int x, y;

            Vector2Int(int x = 0, int y = 0);
            Vector2Int(const glm::ivec2 &vec);
            Vector2Int(const Vector2 &vec);

            // Operators
            Vector2Int operator+(const Vector2Int &other);
            Vector2Int &operator+=(const Vector2Int &other);
            Vector2Int operator-(const Vector2Int &other);
            Vector2Int &operator-=(const Vector2Int &other);
            Vector2Int operator*(int other);
            Vector2Int &operator*=(int other);
            Vector2Int operator*(const Vector2Int &other);
            Vector2Int &operator*=(const Vector2Int &other);
            Vector2Int operator/(int other);
            Vector2Int &operator/=(int other);
            bool operator==(const Vector2Int &other);
            bool operator!=(const Vector2Int &other);
            operator Vector2();

            // Returns the magnitude of the Vector2Int
            float GetMagnitude();
            // Scales the Vector2Int to 8 direction
            Vector2Int &EightDirection();
            // Returns the 8 direction version of the Vector2Int
            Vector2Int EightDirectioned();
            // Returns the normalized version of the Vector2Int
            Vector2 Normalized();

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

        struct Vector4Int
        {
            int x, y, z, w;

            Vector4Int(int x = 0, int y = 0, int z = 0, int w = 1);
            Vector4Int(const glm::ivec4 &vec);
            Vector4Int(const Vector2Int &vec);
            Vector4Int(const Vector4 &vec);

            // Operators
            Vector4Int operator+(const Vector4Int &other);
            Vector4Int &operator+=(const Vector4Int &other);
            Vector4Int operator-(const Vector4Int &other);
            Vector4Int &operator-=(const Vector4Int &other);
            Vector4Int operator*(int other);
            Vector4Int &operator*=(int other);
            Vector4Int operator*(const Vector4Int &other);
            Vector4Int &operator*=(const Vector4Int &other);
            Vector4Int operator/(int other);
            Vector4Int &operator/=(int other);
            bool operator==(const Vector4Int &other);
            bool operator!=(const Vector4Int &other);

            operator Vector2Int();
            operator Vector4();

            // Returns the magnitude of the Vector2Int
            float GetMagnitude();
            // Scales the Vector2Int to 8 direction
            Vector2Int &EightDirection();
            // Returns the 8 direction version of the Vector2Int
            Vector2Int EightDirectioned();
            // Returns the normalized version of the Vector2Int
            Vector2 Normalized();

            // Returns a Vector4(0, 0, 0, 0)
            static Vector4Int Zero();
            // Returns a Vector4(1, 1, 1, 1)
            static Vector4Int One();
        };

#pragma endregion

#pragma region Advanced Functions

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
        /// <returns>Min, max or in between float</returns>
        float Clamp(float value, float min, float max);

        /// <summary>
        /// Clamps the Vector2 magnitude between min and max values
        /// </summary>
        /// <param name="value:">Vector to clamp</param>
        /// <param name="min:">Min magnitude</param>
        /// <param name="max:">Max magnitude</param>
        /// <returns>Min, max or in between Vector2</returns>
        Vector2 ClampVectorMagnitude(Vector2 vector, float min, float max);

        /// <summary>
        /// Clamps the Vector3 magnitude between min and max values
        /// </summary>
        /// <param name="value:">Vector to clamp</param>
        /// <param name="min:">Min magnitude</param>
        /// <param name="max:">Max magnitude</param>
        /// <returns>Min, max or in between Vector3</returns>
        Vector3 ClampVectorMagnitude(Vector3 vector, float min, float max);

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
        /// Moves the value towards the target value by maxDelta. if the maxDelta is less than the distance between current and target, it returns the target value.
        /// </summary>
        /// <param name="current:">Current value</param>
        /// <param name="target:">Target value</param>
        /// <param name="maxDelta:">Max delta to move</param>
        /// <returns>A value between current and target</returns>
        float MoveTowards(float current, float target, float maxDelta);

        /// <summary>
        /// Moves the vector towards the target vector by maxDeltaMagnitude. if the maxDeltaMagnitude is less than the distance between current and target, it returns the target vector.
        /// </summary>
        /// <param name="current:">Current vector</param>
        /// <param name="target:">Target vector</param>
        /// <param name="maxDeltaMagnitude:">Max normalized delta multiplier to move</param>
        /// <returns>A vector between current and target</returns>
        Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDeltaMagnitude);

        /// <summary>
        /// Moves the vector towards the target vector by maxDeltaMagnitude. if the maxDeltaMagnitude is less than the distance between current and target, it returns the target vector.
        /// </summary>
        /// <param name="current:">Current vector</param>
        /// <param name="target:">Target vector</param>
        /// <param name="maxDeltaMagnitude:">Max normalized delta multiplier to move</param>
        /// <returns>A vector between current and target</returns>
        Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDeltaMagnitude);

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
        bool IsSubString(const char *string1, const char *string2);

        /// <summary>
        /// Splits a string into a vector of strings using the given delimiter.
        /// </summary>
        /// <param name="string">String to split</param>
        /// <param name="delimiter">Delimeter to use</param>
        /// <returns>A vector of strings split with delimeter</returns>
        vector<string> SplitString(const string &stringToSplit, const string &delimiter);

        /// <summary>
        /// Converts a string to an integer.
        /// </summary>
        /// <param name="stringToConvert">String to convert, should include only integers and - sign on start</param>
        /// <returns>Integer in the string</returns>
        int StringToInt(const string &stringToConvert);

        /// <summary>
        /// Prints the matrix in a readable format to console. Uses printf.
        /// </summary>
        /// <param name="matrix"></param>
        void PrintMatrix4(const Matrix4 &matrix, string name = "");

#pragma endregion
    }
}

using namespace GamEncin::MathYaman;
