#include "Encin/Encin.h"

namespace GamEncin
{
#pragma region VBO

    VBO::VBO(vector<Vector3> vertices)
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);
    }

    void VBO::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void VBO::Update(vector<Vector3> vertices)
    {
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), vertices.data(), GL_STATIC_DRAW);
    }

    void VBO::Delete()
    {
        glDeleteBuffers(1, &id);
    }

#pragma endregion

#pragma region VAO

    VAO::VAO()
    {
        glGenVertexArrays(1, &id);
    }

    void VAO::LinkAttributes(GLuint layout, GLuint numComponents, GLenum type, GLuint offsetInBytes)
    {
        GLsizei stride = 2 * sizeof(Vector3); //TODO position + color
        GLvoid* offsetVar = (GLvoid*) (offsetInBytes);

        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offsetVar);
        // layout: location of the vertex attribute in the shader, like 0 for position, 1 for color
        //in shader, layout(location = 0) in vec3 position;
        //numComponents: how many components does the attribute have, like 3 for position, 4 for color
        //type: type of the data, like GL_FLOAT for position, GL_UNSIGNED_INT for color
        //stride: size of the vertex data structure, like sizeof(Vertex)
        //offset: where the attribute starts in the vertex data structure, like color comes after position, so offset is the size of position
        glEnableVertexAttribArray(layout); //like binding the format we want to read buffer data
    }

    void VAO::Bind()
    {
        glBindVertexArray(id);
    }

    void VAO::Delete()
    {
        glDeleteVertexArrays(1, &id);
    }

#pragma endregion

#pragma region IBO

    IBO::IBO(vector<GLuint> indices)
    {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    }

    void IBO::Bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    }

    void IBO::Delete()
    {
        glDeleteBuffers(1, &id);
    }

#pragma endregion

#pragma region Shader

    Shader::Shader(const char* vertexFile, const char* fragmentFile)
    {
        string vertexCode = Input::GetFileContents(vertexFile);
        string fragmentCode = Input::GetFileContents(fragmentFile);

        const char* vertexSource = vertexCode.c_str();
        const char* fragmentSource = fragmentCode.c_str();

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //create the shader object
        glShaderSource(vertexShader, 1, &vertexSource, NULL); //assign code to shader object
        glCompileShader(vertexShader); //compile the shader to machine code
        CheckShaderErrors(vertexShader, "VERTEX");

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
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

        transformMatrixVarId = glGetUniformLocation(id, "transformMatrix");
        objPositionVarId = glGetUniformLocation(id, "objPosition");
        objRotationVarId = glGetUniformLocation(id, "objRotation");
        objScaleVarId = glGetUniformLocation(id, "objScale");
    }

    void Shader::Use()
    {
        glUseProgram(id);
    }

    void Shader::Delete()
    {
        glDeleteProgram(id);
    }

    void Shader::CheckShaderErrors(GLuint shader, const char* type)
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
