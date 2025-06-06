#include "Encin/Encin.h"

namespace GamEncin
{
#pragma region GLArrayObject

    GLArrayObject::GLArrayObject(unsigned int strideSize)
    {
        glGenVertexArrays(1, &id);
        this->strideSize = strideSize;
    }

    void GLArrayObject::LinkAttribute(unsigned int layout, unsigned int numComponents, unsigned int type, unsigned int offsetInBytes)
    {
        GLvoid* offsetVar = (GLvoid*) (offsetInBytes);

        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, strideSize, offsetVar);
        // layout: location of the vertex attribute in the shader, like 0 for position, 1 for color
        //in shader, layout(location = 0) in vec3 position;
        //numComponents: how many components does the attribute have, like 3 for position, 4 for color
        //type: type of the data, like GL_FLOAT for position, GL_UNSIGNED_INT for color
        //stride: size of the vertex data structure, like sizeof(Vertex)
        //offset: where the attribute starts in the vertex data structure, like color comes after position, so offset is the size of position

        glEnableVertexAttribArray(layout); //like binding the format we want to read buffer data
    }

    void GLArrayObject::LinkIntegerAttribute(unsigned int layout, unsigned int numComponents, unsigned int type, unsigned int offsetInBytes)
    {
        GLvoid* offsetVar = (GLvoid*) (offsetInBytes);

        glVertexAttribIPointer(layout, numComponents, type, strideSize, offsetVar);

        glEnableVertexAttribArray(layout);
    }

    void GLArrayObject::Bind()
    {
        glBindVertexArray(id);
    }

    void GLArrayObject::Delete()
    {
        glDeleteVertexArrays(1, &id);
    }

#pragma endregion

#pragma region Shader

    Shader::Shader(const char* vertexFile, const char* fragmentFile)
    {
        string vertexCode = Input::GetFileContents(vertexFile);
        string fragmentCode = Input::GetFileContents(fragmentFile);

        const char* vertexSource = vertexCode.c_str();
        const char* fragmentSource = fragmentCode.c_str();

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //create the shader object
        glShaderSource(vertexShader, 1, &vertexSource, NULL); //assign code to shader object
        glCompileShader(vertexShader); //compile the shader to machine code
        CheckShaderErrors(vertexShader, "VERTEX");

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
        glCompileShader(fragmentShader);
        CheckShaderErrors(fragmentShader, "FRAGMENT");

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);
        CheckShaderErrors(id, "PROGRAM");

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        //shaders already compiled and linked to the program, so we can delete them


        viewMatrixVarId = glGetUniformLocation(id, "viewMatrix");
        projectionMatrixVarId = glGetUniformLocation(id, "projectionMatrix");
    }

    void Shader::Use()
    {
        glUseProgram(id);
    }

    void Shader::Delete()
    {
        glDeleteProgram(id);
    }

    void Shader::CheckShaderErrors(unsigned int shader, const char* type)
    {
        GLint hasCompiled;
        char infoLog[1024];

        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                Application::Stop(ShaderCompilationErr, infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
            if(hasCompiled == GL_FALSE)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                Application::Stop(ShaderLinkingErr, infoLog);
            }
        }
    }

#pragma endregion
}
