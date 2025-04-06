#pragma once
#include "Encin/Tools/MathYaman.h"

#include "GLAD/glad.h"

//TODO BE CAREFUL WITH THESE IN THE VERTEX SHADER
#define POSITION_VBO_LAYOUT 0
#define COLOR_VBO_LAYOUT 1

namespace GamEncin
{
    struct VBO
    {
    public:
        GLuint id;

        VBO(vector<Vector3> vertices);
        void Bind();
        void Update(vector<Vector3> vertices);
        void Delete();
    };

    struct VAO
    {
    public:
        GLuint id;

        VAO();
        //one attribute is one piece of data that is passed to the vertex shader for one vertex, like position, color, normal, etc.
        void LinkAttributes(GLuint layout, GLuint numComponents, GLenum type, GLuint offsetInBytes);
        void Bind();
        void Delete();
    };

    struct IBO
    {
    public:
        GLuint id;

        IBO(vector<GLuint> indices);
        void Bind();
        void Delete();
    };

    struct Shader
    {
    public:
        GLuint id,
            transformMatrixVarId,
            objPositionVarId,
            objRotationVarId,
            objScaleVarId;

        // Constructor that build the Shader Program from 2 different shaders
        Shader(const char* vertexFile, const char* fragmentFile);
        void Use();
        void Delete();

    private:
        void CheckShaderErrors(GLuint shader, const char* type);
    };
}
