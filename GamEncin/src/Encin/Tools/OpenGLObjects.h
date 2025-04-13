#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

#include "GLAD/glad.h"

//TODO BE CAREFUL WITH THESE IN THE VERTEX SHADER
#define SSBO_MODEL_MATRICES_BINDING 0

#define VBO_OBJECT_ID_LAYOUT 0
#define VBO_POSITION_LAYOUT 1
#define VBO_COLOR_LAYOUT 2

namespace GamEncin
{
    struct VAO
    {
    public:
        unsigned int id;
        unsigned int strideSize;

        VAO(unsigned int strideSize);
        //one attribute is one piece of data that is passed to the vertex shader for one vertex, like position, color, normal, etc.
        void LinkAttribute(unsigned int layout, unsigned int numComponents, unsigned int type, unsigned int offsetInBytes);
        void Bind();
        void Delete();
    };

    struct VBO
    {
    public:
        unsigned int id;

        VBO();
        void Bind();
        void Update(vector<RawVertex> vertices);
        void Delete();
    };

    struct IBO
    {
    public:
        unsigned int id;

        IBO();
        void Bind();
        void Update(vector<unsigned int> indices);
        void Delete();
    };

    struct SSBO
    {
    public:
        unsigned int id;

        SSBO();
        void Bind();
        void Update(vector<Matrix4> modelMatrices);
        void Delete();
    };

    struct Shader
    {
    public:
        unsigned int id,
            viewMatrixVarId,
            projectionMatrixVarId;

        Shader(const char* vertexFile, const char* fragmentFile);
        void Use();
        void Delete();

    private:
        void CheckShaderErrors(unsigned int shader, const char* type);
    };
}
