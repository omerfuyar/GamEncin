#pragma once
#include "Encin/Items/Components/Component.h"

namespace GamEncin
{
    class Mesh : public Component
    {
    public:
        Mesh(Object* obj);

        void SetMeshData(MeshData* meshData);
        void SetTexture(Texture* texture);

        MeshData* const GetMeshData();
        Texture* const GetTexture();
        Matrix4* const GetModelMatrix();

    protected:
        MeshData* meshData = nullptr;
        Texture* texture = nullptr;
        Matrix4* modelMatrix = nullptr;
    };
}
