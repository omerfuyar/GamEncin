#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/OpenGLObjects.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Mesh : public Component
    {
    public:
        Mesh(Object* obj);
        ~Mesh();

        MeshData meshData;
        Texture* meshTexture;

        void SetMeshData(MeshData* data);
        void SetMeshTexture(Texture* texture);
    };
}
