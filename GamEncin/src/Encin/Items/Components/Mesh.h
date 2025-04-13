#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/OpenGLObjects.h"
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

        void SetMeshData(MeshData* data);
    };
}
