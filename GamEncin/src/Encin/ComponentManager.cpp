#include "ECS.h"

namespace GamEncin
{
#pragma region Transform

    void Transform::Translate(Vector3 amount)
    {
        position += amount;
    }

    void Transform::Rotate(Vector3 amount)
    {
        rotation += amount;
    }

    void Transform::Scale(Vector3 amount)
    {
        scale += amount;
    }

#pragma endregion

#pragma region PsychicsBody



#pragma endregion

#pragma region Renderer

    void Renderer::SendVerticesDataToBuffer(vector<Vector3> vertices)
    {
        float* verticesFloatArr = Vector3::VerticesVectorToFloatArr(vertices);

        size_t arrLength = vertices.size() * 3; // float count

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arrLength, verticesFloatArr, GL_DYNAMIC_DRAW);
        // copy the vertex data into the buffer's memory

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) 0);
        // attribute start pos, arrLength floats of data, not normalized, stride: 3 floats (for triangles), offset: 0

        glEnableVertexAttribArray(0); // enable the attribute at location 0
    }

#pragma endregion
}
