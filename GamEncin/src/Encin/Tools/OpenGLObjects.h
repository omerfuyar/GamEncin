#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

#include "GLAD/glad.h"

//TODO BE CAREFUL WITH THESE IN THE VERTEX SHADER
#define SSBO_MODEL_MATRICES_BINDING 0
#define SSBO_TEXTURE_HANDLES_BINDING 1

#define VBO_OBJECT_ID_LAYOUT 0
#define VBO_POSITION_LAYOUT 1
#define VBO_NORMAL_LAYOUT 2
#define VBO_UV_LAYOUT 3

namespace GamEncin
{
    struct GLArrayObject
    {
    public:
        unsigned int id;
        unsigned int strideSize;

        GLArrayObject(unsigned int strideSize);
        //one attribute is one piece of data that is passed to the vertex shader for one vertex, like position, color, normal, etc.
        void LinkAttribute(unsigned int layout, unsigned int numComponents, unsigned int type, unsigned int offsetInBytes);
        void LinkIntegerAttribute(unsigned int layout, unsigned int numComponents, unsigned int type, unsigned int offsetInBytes);
        void Bind();
        void Delete();
    };

    template <typename T>
    struct GLBufferObject
    {
    public:
        GLBufferObject(unsigned int typeOfBufferObject, unsigned int typeOfDraw)
        {
            this->typeOfBufferObject = typeOfBufferObject;
            this->typeOfDraw = typeOfDraw;
            glGenBuffers(1, &this->id);
        }

        void Update(vector<T> vectorOfType)
        {
            Bind();
            glBufferData(this->typeOfBufferObject, vectorOfType.size() * sizeof(T), vectorOfType.data(), this->typeOfDraw);
        }

        void Bind()
        {
            glBindBuffer(typeOfBufferObject, id);
        }

        void Delete()
        {
            glDeleteBuffers(1, &id);
        }

    protected:
        unsigned int id;
        unsigned int typeOfBufferObject;
        unsigned int typeOfDraw;
    };

    template <typename T>
    struct GLShaderStorageBufferObject : GLBufferObject<T>
    {
    public:
        GLShaderStorageBufferObject(unsigned int locationToBind, unsigned int typeOfDraw) : GLBufferObject<T>(GL_SHADER_STORAGE_BUFFER, typeOfDraw)
        {
            this->locationToBind = locationToBind;
        }

        void Bind()
        {
            glBindBuffer(this->typeOfBufferObject, this->id);
            glBindBufferBase(this->typeOfBufferObject, this->locationToBind, this->id);
        }

    protected:
        unsigned int locationToBind;
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
