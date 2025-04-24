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
        if(!data)
        {
            Application::PrintLog(NullPointerErr, "Mesh data trying to set is null");
            return;
        }

        if(meshData)
        {
            meshData->DeleteData();
        }

        meshData = data;
    }

    void Mesh::SetMeshTexture(Texture* texture)
    {
        meshTexture = texture;
    }

    MeshData* const Mesh::GetMeshData()
    {
        return meshData;
    }

    Texture* const Mesh::GetMeshTexture()
    {
        return meshTexture;
    }
}
