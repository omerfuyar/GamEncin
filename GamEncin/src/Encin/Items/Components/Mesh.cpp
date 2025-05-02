#include "Encin/Encin.h"

namespace GamEncin
{

#pragma region Primitives 

    Mesh::Mesh(Object* obj) : Component(obj)
    {
    }

    Mesh::~Mesh()
    {
        Renderer::RemoveMesh(this->meshData);
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

    bool Mesh::HasChanged()
    {
        return hasChanged;
    }

    MeshData* const Mesh::GetMeshData()
    {
        return meshData;
    }
}
