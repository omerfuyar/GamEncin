#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Items/OpenGLObjects.h"
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

#include <GLAD/glad.h>

namespace GamEncin
{
    class Mesh : public Component
    {
    public:
        Mesh(Object* obj);
        ~Mesh();

        MeshData meshData;

        VAO* vao = nullptr;
        VBO* vbo = nullptr;
        IBO* ebo = nullptr;

        void SetMeshData(MeshData* data);
        void Initialize();
        void Draw();
    };
}
