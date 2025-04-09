#include "Encin/Encin.h"

namespace GamEncin
{

#pragma region Primitives 

    void Mesh::SetMeshData(MeshData* data)
    {
        meshData.DeleteData();
        meshData = *data;
    }

    Mesh::Mesh(Object* object) : Component(object)
    {
        if(Application::isRunning)
        {
            Initialize();
        }

        object->scene->renderer->AddMesh(this);
    }

    Mesh::~Mesh()
    {
        object->scene->renderer->RemoveMesh(this);

        if(vao)
        {
            vao->Delete();
            delete vao;
        }
        if(vbo)
        {
            vbo->Delete();
            delete vbo;
        }
        if(ebo)
        {
            ebo->Delete();
            delete ebo;
        }
    }

    void Mesh::Initialize()
    {
        vao = new VAO();
        vbo = new VBO(meshData.GetRawVertexArray());
        ebo = new IBO(meshData.GetIndiceArray());
    }

    void Mesh::Draw()
    {
        vao->Bind();
        vbo->Bind();
        ebo->Bind();

        vao->LinkAttributes(POSITION_VBO_LAYOUT, 3, GL_FLOAT, 0);
        vao->LinkAttributes(COLOR_VBO_LAYOUT, 4, GL_FLOAT, sizeof(Vector3));

        glDrawElements(GL_TRIANGLES, meshData.faces.size() * 3, GL_UNSIGNED_INT, 0);
    }
}
