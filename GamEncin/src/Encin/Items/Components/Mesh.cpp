#include "Encin/Encin.h"

namespace GamEncin
{

#pragma region Primitives 

    void Vertex::SetID(unsigned int id)
    {
        this->id = id;
    }

    void Vertex::SetPosition(Vector3 position)
    {
        this->position = position;
    }

    void Vertex::SetNormal(Vector3 normal)
    {
        this->normal = normal;
    }

    void Vertex::SetTexture(Vector2 texture)
    {
        this->texture = texture;
    }

    void Vertex::SetColor(Vector4 color)
    {
        this->color = color;
    }

    void Vertex::AddFace(Face* face)
    {
        faces.push_back(face);
    }

    void Vertex::AddEdge(Edge* edge)
    {
        edges.push_back(edge);
    }

    void Edge::SetID(unsigned int id)
    {
        this->id = id;
    }

    void Face::SetID(unsigned int id)
    {
        this->id = id;
    }

#pragma endregion

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
