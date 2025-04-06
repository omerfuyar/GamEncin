#include "Encin/Encin.h"

namespace GamEncin
{

    void Mesh::SetShape(Shape* newShape)
    {
        vertices = newShape->vertices;
        indices = newShape->indices;
    }

    Mesh::Mesh(Object* object) : Component(object)
    {
        if(Application::isRunning)
        {
            Initialize();
        }

        SetShape(new Cube());
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
        vbo = new VBO(vertices);
        ebo = new IBO(indices);
    }

    void Mesh::Draw()
    {
        vao->Bind();
        vbo->Bind();
        ebo->Bind();

        vao->LinkAttributes(POSITION_VBO_LAYOUT, 3, GL_FLOAT, 0);
        vao->LinkAttributes(COLOR_VBO_LAYOUT, 3, GL_FLOAT, sizeof(Vector3));

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}
