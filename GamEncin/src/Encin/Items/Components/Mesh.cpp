#include "Encin/Encin.h"

namespace GamEncin
{

#pragma region Primitives 

    Mesh::Mesh(Object* obj) : Component(obj)
    {
    }

    Mesh::~Mesh()
    {
        Renderer::RemoveMesh(this);
    }

    void Mesh::SetMeshData(MeshData* data)
    {
        meshData.DeleteData();
        meshData = *data;
    }

    void Mesh::SetMeshTexture(Texture* texture)
    {
        meshTexture = texture;
    }
}
