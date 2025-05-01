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

    void Mesh::SetChanged(bool value)
    {
        hasChanged = value;
    }

    void Mesh::SetMeshData(MeshData* data)
    {
        if(!data)
        {
            Application::PrintLog(NullPointerErr, "Mesh data trying to set is null");
            return;
        }

        meshData = data;

        SetChanged(true);
    }

    void Mesh::SetMeshTexture(Texture* texture)
    {
        if(!texture)
        {
            Application::PrintLog(NullPointerErr, "Mesh texture trying to set is null");
            return;
        }

        meshTexture = texture;

        SetChanged(true);
    }

    bool Mesh::HasChanged()
    {
        return hasChanged;
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
