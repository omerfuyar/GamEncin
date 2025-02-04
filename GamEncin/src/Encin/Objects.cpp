#include "Encin.h"
#include <algorithm>
#include <iostream>

namespace GamEncin
{
    Scene::Scene()
    {
        Application::GetInstance().scenes.push_back(this);
        std::cout << "Scene added\n";
    }

    void Object::SendVerticesDataToBuffer(vector<Vector3> vertices)
    {
        float* verticesFloatArr = Vector3::VerticesVectorToFloatArr(vertices);

        size_t arrLength = vertices.size() * 3; // float count

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arrLength, verticesFloatArr, GL_DYNAMIC_DRAW);
        // copy the vertex data into the buffer's memory

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*) 0);
        // attribute start pos, arrLength floats of data, not normalized, stride: 3 floats (for triangles), offset: 0

        glEnableVertexAttribArray(0); // enable the attribute at location 0
    }

    Object& Scene::CreateObject()
    {
        Object* object = new Object();
        objects.push_back(object);
        return *object;
    }

    void Scene::AddObject(Object& object)
    {
        objects.push_back(&object);
    }

    void Scene::RemoveObject(Object& object)
    {
        for(int i = 0; i < objects.size(); i++)
        {
            if(objects[i] == &object)
            {
                objects.erase(objects.begin() + i);
                break;
            }
        }

        auto obj = std::find(objects.begin(), objects.end(), &object);

        if(obj != objects.end())
            objects.erase(obj);
        else
            Application::GetInstance().End(ObjCouldNotFind);
    }

    void Scene::Clear()
    {
        objects.clear();
    }

    void Scene::Awake()
    {
        std::cout << objects.size();
        for(Object* object : objects)
        {
            object->Awake();
        }
    }

    void Scene::Start()
    {
        for(Object* object : objects)
        {
            object->Start();
        }
    }

    void Scene::Update()
    {
        for(Object* object : objects)
        {
            object->Update();
        }
    }

    void Scene::FixUpdate()
    {
        for(auto object : objects)
        {
            object->FixUpdate();
        }
    }
}
