#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/OpenGLObjects.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Mesh : public Component
    {
    public:
        Mesh(Object* obj);
        ~Mesh();

        void SetMeshData(MeshData* data);
        void SetMeshTexture(Texture* texture);

        MeshData* const GetMeshData();
        Texture* const GetMeshTexture();

    private:
        MeshData* meshData = nullptr;
        Texture* meshTexture = nullptr;
    };
}
