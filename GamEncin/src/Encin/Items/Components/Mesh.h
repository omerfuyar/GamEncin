#pragma once
#include "Encin/Items/Components/Component.h"

namespace GamEncin
{
    class Mesh : public Component
    {
    public:
        Mesh(Object *obj);
        ~Mesh();

        void SetMeshData(MeshData *meshData);
        void SetTexture(Texture *texture);

        MeshData *const GetMeshData();
        Texture *const GetTexture();
        Matrix4 *const GetModelMatrix();

    protected:
        MeshData *meshData = MeshBuilder::CreateMeshData({}, {});
        Texture *texture = nullptr;
        Matrix4 *modelMatrix = nullptr;
    };
}
