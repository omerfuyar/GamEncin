#include "Encin/Encin.h"

namespace GamEncin
{

#pragma region Primitives 

    Mesh::Mesh(Object* obj) : Component(obj)
    {
        //if(Application::isRunning)
        //{
        //    Initialize();
        //} TODO

        Renderer::AddMesh(this);
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
}
