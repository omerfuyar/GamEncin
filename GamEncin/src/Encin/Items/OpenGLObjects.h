#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

#include "GLAD/glad.h"

//TODO BE CAREFUL WITH THESE IN THE VERTEX SHADER
#define POSITION_VBO_LAYOUT 0
#define COLOR_VBO_LAYOUT 1

namespace GamEncin
{
    struct VBO
    {
    public:
        unsigned int id;

        VBO(vector<RawVertex> vertices);
        void Bind();
        void Update(vector<RawVertex> vertices);
        void Delete();
    };

    struct VAO
    {
    public:
        unsigned int id;

        VAO();
        //one attribute is one piece of data that is passed to the vertex shader for one vertex, like position, color, normal, etc.
        void LinkAttributes(unsigned int layout, unsigned int numComponents, unsigned int type, unsigned int offsetInBytes);
        void Bind();
        void Delete();
    };

    struct IBO
    {
    public:
        unsigned int id;

        IBO(vector<unsigned int> indices);
        void Bind();
        void Delete();
    };

    struct Shader
    {
    public:
        unsigned int id,
            transformMatrixVarId,
            objPositionVarId,
            objRotationVarId,
            objScaleVarId;

        Shader(const char* vertexFile, const char* fragmentFile);
        void Use();
        void Delete();

    private:
        void CheckShaderErrors(unsigned int shader, const char* type);
    };
}
