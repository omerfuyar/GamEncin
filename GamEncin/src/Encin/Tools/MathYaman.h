#pragma once
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <map>
#include <string>
#include <vector>

using std::array;
using std::map;
using std::string;
using std::vector;

namespace GamEncin
{
    namespace MathYaman
    {
#pragma region Constants

        //Euler, PI, Earth's gravity, Gravitational constant
        const float E = 2.7182817F,
            PI = 3.1415927F,
            GRAVITY = 9.80665F,
            G = 6.67F / 1e11F;

#pragma endregion

#pragma region Basic Functions

        // Returns true if the value is between min and max values, inclusive
        inline bool Between(float value, float min, float max)
        {
            return value >= min && value <= max;
        }

        // Returns the absolute value of the float
        inline float Abs(float value)
        {
            return value < 0 ? -value : value;
        }

        // Returns the rounded value of the float
        inline int Round(float value)
        {
            return (int) (value + 0.5);
        }

        // Returns the root of the float value
        inline float Root(float value, float root)
        {
            return pow(value, 1 / root);
        }

        // Return square root of the float value
        inline float Sqrt(float value)
        {
            return sqrt(value);
        }

        // Returns the cube root of the float value
        inline float Cbrt(float value)
        {
            return cbrt(value);
        }

        // Returns the power of the base to the exponent
        inline float Power(float base, float exponent)
        {
            return pow(base, exponent);
        }

        // Returns the logarithm of the value to the base
        inline float Log(float value, float base)
        {
            return log(value) / log(base);
        }

        // Returns the logarithm of the value to the base 10
        inline float Log10(float value)
        {
            return log10(value);
        }

        // Returns the natural logarithm of the value
        inline float LogE(float value)
        {
            return log(value);
        }

        // Converts the radian value to degree
        inline float Rad2Deg(float radian)
        {
            return radian * 180 / PI;
        }

        // Converts the degree value to radians
        inline float Deg2Rad(float degree)
        {
            return degree * PI / 180;
        }

        // Returns the sine of the radian value
        inline float SinRad(float radian)
        {
            return sin(radian);
        }

        // Returns the sine of the degree value
        inline float SinDeg(float degree)
        {
            return sin(Deg2Rad(degree));
        }

        // Returns the cosine of the radian value
        inline float CosRad(float radian)
        {
            return cos(radian);
        }

        // Returns the cosine of the degree value
        inline float CosDeg(float degree)
        {
            return cos(Deg2Rad(degree));
        }

        // Returns the tangent of the radian value
        inline float TanRad(float radian)
        {
            return tan(radian);
        }

        // Returns the tangent of the degree value
        inline float TanDeg(float degree)
        {
            return tan(Deg2Rad(degree));
        }

        // Returns the cotangent of the radian value
        inline float CotRad(float radian)
        {
            return 1 / TanRad(radian);
        }

        // Returns the cotangent of the degree value
        inline float CotDeg(float degree)
        {
            return 1 / TanDeg(degree);
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

            // Returns the magnitude of the Vector2
            inline float GetMagnitude()
            {
                return Sqrt(x * x + y * y);
            }

            // Scales the Vector2 to magnitude of 1 and returns it
            inline Vector2 Normalize()
            {
                float magnitude = GetMagnitude();
                x /= magnitude;
                y /= magnitude;
                return Vector2(x, y);
            }

            // Returns the normalized version of the Vector2
            inline Vector2 Normalized()
            {
                Vector2 result = Vector2(x, y);
                result.Normalize();
                return result;
            }

            // Returns the glm vec2 version of the Vector2
            inline glm::vec2 ToGLMVec2()
            {
                return glm::vec2(x, y);
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
            Vector3(const Vector2& vec, float z = 0) : x(vec.x), y(vec.y), z(z) {}
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

            // Returns the magnitude of the Vector3
            inline float GetMagnitude()
            {
                return Sqrt(x * x + y * y + z * z);
            }

            // Scales and returns the Vector3 magnitude of 1
            inline Vector3 Normalize()
            {
                float magnitude = GetMagnitude();
                x /= magnitude;
                y /= magnitude;
                z /= magnitude;
                return Vector3(x, y, z);
            }

            // Returns the normalized version of the Vector2
            inline Vector3 Normalized()
            {
                Vector3 result = Vector3(x, y, z);
                result.Normalize();
                return result;
            }

            /// <summary>
            /// Returns the cross product of two Vector3
            /// </summary>
            /// <param name="otherVec: ">Vector to take cross</param>
            inline Vector3 Cross(const Vector3& otherVec) const
            {
                return Vector3(
                    y * otherVec.z - z * otherVec.y,
                    z * otherVec.x - x * otherVec.z,
                    x * otherVec.y - y * otherVec.x
                );
            }

            // Returns the glm vec3 version of the Vector3
            inline glm::vec3 ToGLMvec3()
            {
                return glm::vec3(x, y, z);
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

        struct Vector2Int
        {
            int x, y;

            Vector2Int(int x = 0, int y = 0) : x(x), y(y) {};
            Vector2Int(const glm::ivec2& vec) : x(vec.x), y(vec.y) {};
            Vector2Int(const Vector2& vec) : x((int) vec.x), y((int) vec.y) {}
            ~Vector2Int() = default;

#pragma region Operators

            inline Vector2Int operator + (const Vector2Int& other)
            {
                return Vector2Int(x + other.x, y + other.y);
            }

            inline Vector2Int operator += (const Vector2Int& other)
            {
                x += other.x;
                y += other.y;
                return Vector2Int(x, y);
            }

            inline Vector2Int operator - (const Vector2Int& other)
            {
                return Vector2Int(x - other.x, y - other.y);
            }

            inline Vector2Int operator -= (const Vector2Int& other)
            {
                x -= other.x;
                y -= other.y;
                return Vector2Int(x, y);
            }

            inline Vector2Int operator *= (const Vector2Int& other)
            {
                x *= other.x;
                y *= other.y;
                return Vector2Int(x, y);
            }

            inline Vector2Int operator * (const int other)
            {
                return Vector2Int(x * other, y * other);
            }

            inline Vector2Int operator *= (const int other)
            {
                x *= other;
                y *= other;
                return Vector2Int(x, y);
            }

            inline Vector2Int operator / (const int other)
            {
                return Vector2Int(x / other, y / other);
            }

            inline Vector2Int operator /= (const int other)
            {
                x /= other;
                y /= other;
                return Vector2Int(x, y);
            }

            inline bool operator == (const Vector2Int& other)
            {
                return x == other.x && y == other.y;
            }

            inline bool operator != (const Vector2Int& other)
            {
                return x != other.x || y != other.y;
            }

#pragma endregion

#pragma region Functions

            // Returns the magnitude of the Vector2Int
            inline float GetMagnitude()
            {
                return Sqrt(x * x + y * y);
            }

            // Scales the Vector2Int to magnitude of 1 and returns it
            inline Vector2 Normalize()
            {
                float magnitude = GetMagnitude();
                x /= magnitude;
                y /= magnitude;
                return Vector2(x, y);
            }

            // Returns the normalized version of the Vector2Int
            inline Vector2 Normalized()
            {
                Vector2 result = Vector2(x, y);
                result.Normalize();
                return result;
            }

            //Returns a Vector2Int(0, 0)
            static inline Vector2Int Zero() { return Vector2Int(0, 0); }
            //Returns a Vector2Int(1, 1)                            
            static inline Vector2Int One() { return Vector2Int(1, 1); }
            //Returns a Vector2Int(0, 1)                           
            static inline Vector2Int Up() { return Vector2Int(0, 1); }
            //Returns a Vector2Int(1, 0)                          
            static inline Vector2Int Right() { return Vector2Int(1, 0); }
            //Returns a Vector2Int(0, -1)
            static inline Vector2Int Down() { return Vector2Int(0, -1); }
            //Returns a Vector2Int(-1, 0)
            static inline Vector2Int Left() { return Vector2Int(-1, 0); }

#pragma endregion
        };

#pragma endregion

#pragma region Advenced Functions

        /// <summary>
        /// Generates a random float
        /// </summary>
        /// <param name="min:">Minimum border, inclusive</param>
        /// <param name="max:">Maximum border, inclusive</param>
        /// <returns>A random float value between min and max values</returns>
        inline float RandomRangeFloat(float min, float max)
        {
            return min + (float) rand() / (float) (RAND_MAX / (max - min));
        }

        /// <summary>
        /// Generates a random integer
        /// </summary>
        /// <param name="min:">Minimum border, inclusive</param>
        /// <param name="max:">Maximum border, inclusive</param>
        /// <returns>A random integer value between min and max values</returns>
        inline int RandomRangeInteger(int min, int max)
        {
            return min + (int) (rand() % (max - min + 1));
        }

        /// <summary>
        /// Generates a random Vector2
        /// </summary>
        /// <returns>A random point in unit circle</returns>
        inline Vector2 RandomVector2()
        {
            return(Vector2(RandomRangeFloat(-1, 1), RandomRangeFloat(-1, 1)));
        }

        /// <summary>
        /// Generates a random Vector3
        /// </summary>
        /// <returns>A random point in unit sphere</returns>
        inline Vector3 RandomVector3()
        {
            return(Vector3(RandomRangeFloat(-1, 1), RandomRangeFloat(-1, 1), RandomRangeFloat(-1, 1)));
        }

        /// <summary>
        /// Generates a random Vector3 direction with magnitude of 1
        /// </summary>
        /// <returns>A normalized random Vector3</returns>
        inline Vector3 RandomDirection()
        {
            return RandomVector3().Normalize();
        }

        // Returns the smaller value between two float values
        inline float Min(float a, float b)
        {
            return a < b ? a : b;
        }

        // Returns the vector with smaller magnitude between two Vector2
        inline Vector2 Min(Vector2 a, Vector2 b)
        {
            return a.GetMagnitude() < b.GetMagnitude() ? a : b;
        }

        // Returns the vector with smaller magnitude between two Vector3
        inline Vector3 Min(Vector3 a, Vector3 b)
        {
            return a.GetMagnitude() < b.GetMagnitude() ? a : b;
        }

        // Returns the greater value between two float values
        inline float Max(float a, float b)
        {
            return a > b ? a : b;
        }

        // Returns the vector with greater magnitude between two Vector2
        inline Vector2 Max(Vector2 a, Vector2 b)
        {
            return a.GetMagnitude() > b.GetMagnitude() ? a : b;
        }

        // Returns the vector with greater magnitude between two Vector3
        inline Vector3 Max(Vector3 a, Vector3 b)
        {
            return a.GetMagnitude() > b.GetMagnitude() ? a : b;
        }

        /// <summary>
        /// Clamps the value between min and max values
        /// </summary>
        /// <param name="value:">Value to restrict</param>
        /// <param name="min:">Min value</param>
        /// <param name="max:">Max value</param>
        /// <returns>Min, max or inbetween float</returns>
        inline float Clamp(float value, float min, float max)
        {
            return value < min ? min : value > max ? max : value;
        }

        /// <summary>
        /// Clamps the Vector2 magnitude between 0 and max value
        /// </summary>
        /// <param name="value:">Vector to restrict</param>
        /// <param name="max:">Max magnitude</param>
        /// <returns>Original vector or a Vector2 with max magnitude</returns>
        inline Vector2 ClampVectorMagnitude(Vector2 vector, float max)
        {
            float magnitude = vector.GetMagnitude();
            return magnitude > max ? vector * (max / magnitude) : vector;
        }

        /// <summary>
        /// Clamps the Vector3 magnitude between 0 and max value
        /// </summary>
        /// <param name="value:">Vector to restrict</param>
        /// <param name="max:">Max magnitude</param>
        /// <returns>Original vector or a Vector3 with max magnitude</returns>
        inline Vector3 ClampVectorMagnitude(Vector3 vector, float max)
        {
            float magnitude = vector.GetMagnitude();
            return magnitude > max ? vector * (max / magnitude) : vector;
        }

        /// <summary>
        /// Linear Interpolation between two float values
        /// </summary>
        /// <param name="startVal:">Start value</param>
        /// <param name="endVal:">End value</param>
        /// <param name="t:">Between 0 and 1. 0 is startVal, 1 is endVal</param>
        /// <returns>The float value between start and end values depending on t</returns>
        inline float Lerp(float startVal, float endVal, float t)
        {
            return startVal + (endVal - startVal) * Clamp(t, 0, 1);
        }

        /// <summary>
        /// Linear Interpolation between two Vector2
        /// </summary>
        /// <param name="startVal:">Start vector</param>
        /// <param name="endVal:">End vector</param>
        /// <param name="t:">Between 0 and 1. 0 is startVec, 1 is endVec</param>
        /// <returns>The Vector2 between start and end vectors depending on t</returns>
        inline Vector2 Lerp(Vector2 startVec, Vector2 endVec, float t)
        {
            return startVec + (endVec - startVec) * Clamp(t, 0, 1);
        }

        /// <summary>
        /// Linear Interpolation between two Vector3
        /// </summary>
        /// <param name="startVal:">Start vector</param>
        /// <param name="endVal:">End vector</param>
        /// <param name="t:">Between 0 and 1. 0 is startVec, 1 is endVec</param>
        /// <returns>The Vector3 between start and end vectors depending on t</returns>
        inline static Vector3 Lerp(Vector3 startVec, Vector3 endVec, float t)
        {
            return startVec + (endVec - startVec) * Clamp(t, 0, 1);
        }

        /// <summary>
        /// Returns the distance between two Vector2 point
        /// </summary>
        /// <param name="startVec:">Start vector</param>
        /// <param name="endVec:">End vector</param>
        /// <returns> </returns>
        inline static float Distance(Vector2 a, Vector2 b)
        {
            return (b - a).GetMagnitude();
        }

        /// <summary>
        /// Returns the distance between two Vector3 point
        /// </summary>
        /// <param name="startVec:">Start vector</param>
        /// <param name="endVec:">End vector</param>
        /// <returns> </returns>
        inline static float Distance(Vector3 a, Vector3 b)
        {
            return (b - a).GetMagnitude();
        }

#pragma endregion
    }
}

using namespace GamEncin::MathYaman;
