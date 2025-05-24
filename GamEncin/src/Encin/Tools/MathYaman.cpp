#include "Encin/Encin.h"

namespace GamEncin
{
    namespace MathYaman
    {
#pragma region Constants

        const double E = 2.7182817;
        const double PI = 3.1415927;
        const double GRAVITY = 9.80665;
        const double G = 6.67 / 1e11;

#pragma endregion

#pragma region Basic Functions

        int Factorial(int n)
        {
            if (n < 0)
                return -1;
            if (n == 0 || n == 1)
                return 1;
            return n * Factorial(n - 1);
        }

        bool Between(float value, float min, float max)
        {
            return value >= min && value <= max;
        }

        float Abs(float value)
        {
            return value < 0 ? -value : value;
        }

        float Sign(float value)
        {
            return value < 0 ? -1.0f : (value > 0 ? 1.0f : 0.0f);
        }

        int Sign(int value)
        {
            return value < 0 ? -1 : (value > 0 ? 1 : 0);
        }

        int Round(float value)
        {
            return (int)(value + 0.5);
        }

        float Root(float value, float root)
        {
            return pow(value, 1 / root);
        }

        float SquareRoot(float value)
        {
            return sqrt(value);
        }

        float CubeRoot(float value)
        {
            return cbrt(value);
        }

        float Power(float base, float exponent)
        {
            return pow(base, exponent);
        }

        float Square(float value)
        {
            return value * value;
        }

        Vector2 Square(Vector2 vec)
        {
            return vec * vec;
        }

        Vector3 Square(Vector3 vec)
        {
            return vec * vec;
        }

        float Cube(float value)
        {
            return value * value * value;
        }

        Vector2 Cube(Vector2 vec)
        {
            return vec * vec * vec;
        }

        Vector3 Cube(Vector3 vec)
        {
            return vec * vec * vec;
        }

        float Log(float value, float base)
        {
            return log(value) / log(base);
        }

        float Log10(float value)
        {
            return log10(value);
        }

        float LogE(float value)
        {
            return log(value);
        }

        float Rad2Deg(float radian)
        {
            return radian * 180 / PI;
        }

        float Deg2Rad(float degree)
        {
            return degree * PI / 180;
        }

        float DegBetweenAngles(float a, float b)
        {
            return Rad2Deg(RadBetweenAngles(a, b));
        }

        float RadBetweenAngles(float a, float b)
        {
            return atan2(SinRad(b - a), CosRad(b - a));
        }

        float SinRad(float radian)
        {
            return sin(radian);
        }

        float SinDeg(float degree)
        {
            return sin(Deg2Rad(degree));
        }

        float CosRad(float radian)
        {
            return cos(radian);
        }

        float CosDeg(float degree)
        {
            return cos(Deg2Rad(degree));
        }

        float TanRad(float radian)
        {
            return tan(radian);
        }

        float TanDeg(float degree)
        {
            return tan(Deg2Rad(degree));
        }

        float ArcTanDeg(float x)
        {
            return Rad2Deg(ArcTanRad(x));
        }

        float ArcTanRad(float x)
        {
            return atan(x);
        }

        float ArcTan2Deg(float x, float y)
        {
            return Rad2Deg(ArcTan2Rad(x, y));
        }

        float ArcTan2Rad(float x, float y)
        {
            return atan2(y, x);
        }

        float CotRad(float radian)
        {
            return 1 / TanRad(radian);
        }

        float CotDeg(float degree)
        {
            return 1 / TanDeg(degree);
        }

#pragma endregion

#pragma region Vectors

#pragma region Vector2

        Vector2::Vector2(float x, float y) : x(x), y(y) {}
        Vector2::Vector2(const glm::vec2 &vec) : x(vec.x), y(vec.y) {}

        Vector2 Vector2::operator+(const Vector2 &other)
        {
            return Vector2(x + other.x, y + other.y);
        }

        Vector2 &Vector2::operator+=(const Vector2 &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2 Vector2::operator-(const Vector2 &other)
        {
            return Vector2(x - other.x, y - other.y);
        }

        Vector2 &Vector2::operator-=(const Vector2 &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2 Vector2::operator*(const float other)
        {
            return Vector2(x * other, y * other);
        }

        Vector2 &Vector2::operator*=(const float other)
        {
            x *= other;
            y *= other;
            return *this;
        }

        Vector2 Vector2::operator*(const Vector2 &other)
        {
            return Vector2(x * other.x, y * other.y);
        }

        Vector2 &Vector2::operator*=(const Vector2 &other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }

        Vector2 Vector2::operator/(const float other)
        {
            return Vector2(x / other, y / other);
        }

        Vector2 &Vector2::operator/=(const float other)
        {
            x /= other;
            y /= other;
            return *this;
        }

        bool Vector2::operator==(const Vector2 &other)
        {
            return x == other.x && y == other.y;
        }

        bool Vector2::operator!=(const Vector2 &other)
        {
            return x != other.x || y != other.y;
        }

        float Vector2::GetMagnitude()
        {
            return SquareRoot(x * x + y * y);
        }

        Vector2 &Vector2::Normalize()
        {
            float magnitude = GetMagnitude();
            x /= magnitude;
            y /= magnitude;
            return *this;
        }

        Vector2 Vector2::Normalized()
        {
            Vector2 result = *this;
            result.Normalize();
            return result;
        }

        glm::vec2 Vector2::ToGLMVec2()
        {
            return glm::vec2(x, y);
        }

        Vector2 Vector2::Zero() { return Vector2(0, 0); }
        Vector2 Vector2::One() { return Vector2(1, 1); }
        Vector2 Vector2::Up() { return Vector2(0, 1); }
        Vector2 Vector2::Right() { return Vector2(1, 0); }
        Vector2 Vector2::Down() { return Vector2(0, -1); }
        Vector2 Vector2::Left() { return Vector2(-1, 0); }

#pragma endregion

#pragma region Vector3

        Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        Vector3::Vector3(const glm::vec3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}
        Vector3::Vector3(const Vector2 &vec) : x(vec.x), y(vec.y), z(0) {}
        Vector3::Vector3(const Vector2 &vec, float z) : x(vec.x), y(vec.y), z(z) {}

        Vector3 Vector3::operator+(const Vector3 &other)
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        Vector3 &Vector3::operator+=(const Vector3 &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vector3 Vector3::operator-(const Vector3 &other)
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        Vector3 &Vector3::operator-=(const Vector3 &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vector3 Vector3::operator*(float other)
        {
            return Vector3(x * other, y * other, z * other);
        }

        Vector3 &Vector3::operator*=(float other)
        {
            x *= other;
            y *= other;
            z *= other;
            return *this;
        }

        Vector3 Vector3::operator*(const Vector3 &other)
        {
            return Vector3(x * other.x, y * other.y, z * other.z);
        }

        Vector3 &Vector3::operator*=(const Vector3 &other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        Vector3 Vector3::operator/(float other)
        {
            return Vector3(x / other, y / other, z / other);
        }

        Vector3 &Vector3::operator/=(float other)
        {
            x /= other;
            y /= other;
            z /= other;
            return *this;
        }

        Vector3 Vector3::operator*(const Matrix4 other)
        {
            glm::vec3 result = other * glm::vec4(x, y, z, 1);
            return Vector3(result.x, result.y, result.z);
        }

        bool Vector3::operator==(const Vector3 &other)
        {
            return x == other.x && y == other.y && z == other.z;
        }

        bool Vector3::operator!=(const Vector3 &other)
        {
            return x != other.x || y != other.y || z != other.z;
        }

        Vector3::operator Vector2()
        {
            return Vector2(x, y);
        }

        float Vector3::GetMagnitude()
        {
            return SquareRoot(x * x + y * y + z * z);
        }

        Vector3 &Vector3::Normalize()
        {
            float magnitude = GetMagnitude();

            if (magnitude == 0)
            {
                return *this;
            }

            x /= magnitude;
            y /= magnitude;
            z /= magnitude;

            return *this;
        }

        Vector3 Vector3::Normalized()
        {
            Vector3 result = *this;
            result.Normalize();
            return result;
        }

        Vector3 Vector3::Cross(const Vector3 &otherVec)
        {
            return Vector3(
                       y * otherVec.z - z * otherVec.y,
                       z * otherVec.x - x * otherVec.z,
                       x * otherVec.y - y * otherVec.x)
                .Normalize();
        }

        float Vector3::Dot(const Vector3 &otherVec)
        {
            return x * otherVec.x + y * otherVec.y + z * otherVec.z;
        }

        glm::vec3 Vector3::ToGLMvec3()
        {
            return glm::vec3(x, y, z);
        }

        Vector3 Vector3::Zero() { return Vector3(0, 0, 0); }
        Vector3 Vector3::One() { return Vector3(1, 1, 1); }
        Vector3 Vector3::Up() { return Vector3(0, 1, 0); }
        Vector3 Vector3::Right() { return Vector3(1, 0, 0); }
        Vector3 Vector3::Down() { return Vector3(0, -1, 0); }
        Vector3 Vector3::Left() { return Vector3(-1, 0, 0); }
        Vector3 Vector3::Forward() { return Vector3(0, 0, 1); }
        Vector3 Vector3::Backward() { return Vector3(0, 0, -1); }

#pragma endregion

#pragma region Vector4

        Vector4::Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        Vector4::Vector4(const glm::vec4 &vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
        Vector4::Vector4(const Vector2 &vec) : x(vec.x), y(vec.y), z(0), w(1) {}
        Vector4::Vector4(const Vector3 &vec) : x(vec.x), y(vec.y), z(vec.z), w(1) {}

        Vector4::operator Vector2()
        {
            return Vector2(x, y);
        }

        Vector4::operator Vector3()
        {
            return Vector3(x, y, z);
        }

        Vector4 Vector4::Zero() { return Vector4(0, 0, 0, 0); }
        Vector4 Vector4::One() { return Vector4(1, 1, 1, 1); }

#pragma endregion

#pragma region Vector2Int

        Vector2Int::Vector2Int(int x, int y) : x(x), y(y) {}
        Vector2Int::Vector2Int(const glm::ivec2 &vec) : x(vec.x), y(vec.y) {}
        Vector2Int::Vector2Int(const Vector2 &vec) : x((int)vec.x), y((int)vec.y) {}

        Vector2Int Vector2Int::operator+(const Vector2Int &other)
        {
            return Vector2Int(x + other.x, y + other.y);
        }

        Vector2Int &Vector2Int::operator+=(const Vector2Int &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        Vector2Int Vector2Int::operator-(const Vector2Int &other)
        {
            return Vector2Int(x - other.x, y - other.y);
        }

        Vector2Int &Vector2Int::operator-=(const Vector2Int &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        Vector2Int Vector2Int::operator*(int other)
        {
            return Vector2Int(x * other, y * other);
        }

        Vector2Int &Vector2Int::operator*=(int other)
        {
            x *= other;
            y *= other;
            return *this;
        }

        Vector2Int Vector2Int::operator*(const Vector2Int &other)
        {
            return Vector2Int(x * other.x, y * other.y);
        }

        Vector2Int &Vector2Int::operator*=(const Vector2Int &other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }

        Vector2Int Vector2Int::operator/(int other)
        {
            return Vector2Int(x / other, y / other);
        }

        Vector2Int &Vector2Int::operator/=(int other)
        {
            x /= other;
            y /= other;
            return *this;
        }

        bool Vector2Int::operator==(const Vector2Int &other)
        {
            return x == other.x && y == other.y;
        }

        bool Vector2Int::operator!=(const Vector2Int &other)
        {
            return x != other.x || y != other.y;
        }

        Vector2Int::operator Vector2()
        {
            return Vector2((float)x, (float)y);
        }

        float Vector2Int::GetMagnitude()
        {
            return SquareRoot((float)(x * x + y * y));
        }

        Vector2Int &Vector2Int::EightDirection()
        {
            x /= (int)Abs((float)x);
            y /= (int)Abs((float)y);
            return *this;
        }

        Vector2Int Vector2Int::EightDirectioned()
        {
            Vector2Int result = *this;
            result.EightDirection();
            return result;
        }

        Vector2 Vector2Int::Normalized()
        {
            Vector2 result = Vector2((float)x, (float)y);
            result.Normalize();
            return result;
        }

        Vector2Int Vector2Int::Zero() { return Vector2Int(0, 0); }
        Vector2Int Vector2Int::One() { return Vector2Int(1, 1); }
        Vector2Int Vector2Int::Up() { return Vector2Int(0, 1); }
        Vector2Int Vector2Int::Right() { return Vector2Int(1, 0); }
        Vector2Int Vector2Int::Down() { return Vector2Int(0, -1); }
        Vector2Int Vector2Int::Left() { return Vector2Int(-1, 0); }

#pragma endregion

#pragma region Vector4Int

        Vector4Int::Vector4Int(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
        Vector4Int::Vector4Int(const glm::ivec4 &vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {}
        Vector4Int::Vector4Int(const Vector2Int &vec) : x(vec.x), y(vec.y), z(0), w(1) {}
        Vector4Int::Vector4Int(const Vector4 &vec) : x((int)vec.x), y((int)vec.y), z((int)vec.z), w((int)vec.w) {}

        Vector4Int Vector4Int::operator+(const Vector4Int &other)
        {
            return Vector4Int(x + other.x, y + other.y, z + other.z, w + other.w);
        }

        Vector4Int &Vector4Int::operator+=(const Vector4Int &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        Vector4Int Vector4Int::operator-(const Vector4Int &other)
        {
            return Vector4Int(x - other.x, y - other.y, z - other.z, w - other.w);
        }

        Vector4Int &Vector4Int::operator-=(const Vector4Int &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
            return *this;
        }

        Vector4Int Vector4Int::operator*(int other)
        {
            return Vector4Int(x * other, y * other, z * other, w * other);
        }

        Vector4Int &Vector4Int::operator*=(int other)
        {
            x *= other;
            y *= other;
            z *= other;
            w *= other;
            return *this;
        }

        Vector4Int Vector4Int::operator*(const Vector4Int &other)
        {
            return Vector4Int(x * other.x, y * other.y, z * other.z, w * other.w);
        }

        Vector4Int &Vector4Int::operator*=(const Vector4Int &other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
            return *this;
        }

        Vector4Int Vector4Int::operator/(int other)
        {
            return Vector4Int(x / other, y / other, z / other, w / other);
        }

        Vector4Int &Vector4Int::operator/=(int other)
        {
            x /= other;
            y /= other;
            z /= other;
            w /= other;
            return *this;
        }

        bool Vector4Int::operator==(const Vector4Int &other)
        {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }

        bool Vector4Int::operator!=(const Vector4Int &other)
        {
            return x != other.x || y != other.y || z != other.z || w != other.w;
        }

        Vector4Int::operator Vector2Int()
        {
            return Vector2Int(x, y);
        }

        Vector4Int::operator Vector4()
        {
            return Vector4((float)x, (float)y, (float)z, (float)w);
        }

        Vector4Int Vector4Int::Zero() { return Vector4Int(0, 0, 0, 0); }

        Vector4Int Vector4Int::One() { return Vector4Int(1, 1, 1, 1); }

#pragma endregion

#pragma endregion

#pragma region Advanced Functions

        float RandomRangeFloat(float min, float max)
        {
            return min + static_cast<float>(rand()) / (RAND_MAX / (max - min));
        }

        int RandomRangeInteger(int min, int max)
        {
            if (min > max)
            {
                int temp = min;
                min = max;
                max = temp;
            }

            return min + rand() % (max - min + 1);
        }

        Vector2 RandomVector2()
        {
            return Vector2(RandomRangeFloat(-1, 1), RandomRangeFloat(-1, 1));
        }

        Vector3 RandomVector3()
        {
            return Vector3(RandomRangeFloat(-1, 1), RandomRangeFloat(-1, 1), RandomRangeFloat(-1, 1));
        }

        Vector2 RandomVector2Direction()
        {
            return RandomVector2().Normalize();
        }

        Vector3 RandomVector3Direction()
        {
            return RandomVector3().Normalize();
        }

        float Min(float a, float b)
        {
            return a < b ? a : b;
        }

        Vector2 Min(Vector2 a, Vector2 b)
        {
            return a.GetMagnitude() < b.GetMagnitude() ? a : b;
        }

        Vector3 Min(Vector3 a, Vector3 b)
        {
            return a.GetMagnitude() < b.GetMagnitude() ? a : b;
        }

        float Max(float a, float b)
        {
            return a > b ? a : b;
        }

        Vector2 Max(Vector2 a, Vector2 b)
        {
            return a.GetMagnitude() > b.GetMagnitude() ? a : b;
        }

        Vector3 Max(Vector3 a, Vector3 b)
        {
            return a.GetMagnitude() > b.GetMagnitude() ? a : b;
        }

        float Clamp(float value, float min, float max)
        {
            return value < min ? min : value > max ? max
                                                   : value;
        }

        Vector2 ClampVectorMagnitude(Vector2 vector, float min, float max)
        {
            float magnitude = vector.GetMagnitude();

            if (magnitude < min)
            {
                return vector.Normalized() * min;
            }
            else if (magnitude > max)
            {
                return vector.Normalized() * max;
            }

            return vector;
        }

        Vector3 ClampVectorMagnitude(Vector3 vector, float min, float max)
        {
            float magnitude = vector.GetMagnitude();
            if (magnitude < min)
            {
                return vector.Normalized() * min;
            }
            else if (magnitude > max)
            {
                return vector.Normalized() * max;
            }
            return vector;
        }

        float Lerp(float startVal, float endVal, float t)
        {
            return startVal + (endVal - startVal) * Clamp(t, 0, 1);
        }

        Vector2 Lerp(Vector2 startVec, Vector2 endVec, float t)
        {
            return startVec + (endVec - startVec) * Clamp(t, 0, 1);
        }

        Vector3 Lerp(Vector3 startVec, Vector3 endVec, float t)
        {
            return startVec + (endVec - startVec) * Clamp(t, 0, 1);
        }

        float MoveTowards(float current, float target, float maxDelta)
        {
            if (Abs(target - current) <= maxDelta)
            {
                return target;
            }

            return current + (target > current ? maxDelta : -maxDelta);
        }

        Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDeltaMagnitude)
        {
            if ((target - current).GetMagnitude() <= maxDeltaMagnitude)
            {
                return target;
            }

            return current + (target - current).Normalize() * maxDeltaMagnitude;
        }

        Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDeltaMagnitude)
        {
            if ((target - current).GetMagnitude() <= maxDeltaMagnitude)
                return target;
            return current + (target - current).Normalize() * maxDeltaMagnitude;
        }

        float Distance(Vector2 a, Vector2 b)
        {
            return (b - a).GetMagnitude();
        }

        float Distance(Vector3 a, Vector3 b)
        {
            return (b - a).GetMagnitude();
        }

        bool IsSubString(const char *str, const char *subStr)
        {
            if (!str || !subStr)
                return false;
            int strLen = strlen(str);
            int subStrLen = strlen(subStr);
            if (strLen < subStrLen)
                return false;
            for (int i = 0; i <= strLen - subStrLen; ++i)
            {
                if (strncmp(&str[i], subStr, subStrLen) == 0)
                    return true;
            }
            return false;
        }

        vector<string> SplitString(const string &stringToSplit, const string &delimiter)
        {
            vector<string> result;
            size_t pos = 0;
            size_t delimiterPos = stringToSplit.find(delimiter);

            while (delimiterPos != string::npos)
            {
                result.push_back(stringToSplit.substr(pos, delimiterPos - pos));
                pos = delimiterPos + delimiter.length();
                delimiterPos = stringToSplit.find(delimiter, pos);
            }

            result.push_back(stringToSplit.substr(pos));

            return result;
        }

        int StringToInt(const string &stringToConvert)
        {
            int result = 0;

            if (stringToConvert.empty())
            {
                Application::PrintLog(TypeMismatchErr, "String to convert is empty");
                return 0;
            }

            if (stringToConvert[0] == '-')
            {
                for (int i = 1; i < stringToConvert.length(); i++)
                {
                    if (stringToConvert[i] < '0' || stringToConvert[i] > '9')
                    {
                        Application::PrintLog(TypeMismatchErr, "String to convert is not a valid integer");
                        return 0;
                    }

                    result = result * 10 + (stringToConvert[i] - '0');
                }

                result = -result;
            }
            else
            {
                for (int i = 0; i < stringToConvert.length(); i++)
                {
                    if (stringToConvert[i] < '0' || stringToConvert[i] > '9')
                    {
                        Application::PrintLog(TypeMismatchErr, "String to convert is not a valid integer");
                        return 0;
                    }

                    result = result * 10 + (stringToConvert[i] - '0');
                }
            }

            return result;
        }

        void PrintMatrix4(const Matrix4 &matrix, string name)
        {
            if (name != "")
                printf("%s:\n", name.c_str());
            else
                printf("Matrix4:\n");

            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    printf("%f ", matrix[i][j]);
                }
                printf("\n");
            }
        }

#pragma endregion
    }
}
