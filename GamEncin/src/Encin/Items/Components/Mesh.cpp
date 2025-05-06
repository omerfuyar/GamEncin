#include "Encin/Encin.h"

namespace GamEncin
{
    Mesh::Mesh(Object* obj) : Component(obj)
    {
        modelMatrix = object->GetTransform()->GetModelMatrix();
    }

    void Mesh::SetMeshData(MeshData* meshData)
    {
        this->meshData = meshData;
    }

    void Mesh::SetTexture(Texture* texture)
    {
        this->texture = texture;
    }

    MeshData* const Mesh::GetMeshData()
    {
        return meshData;
    }

    Texture* const Mesh::GetTexture()
    {
        return texture;
    }

    Matrix4* const Mesh::GetModelMatrix()
    {
        return modelMatrix;
    }
}
