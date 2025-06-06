#include "Encin/Encin.h"

namespace GamEncin
{
    Scene::Scene(string name)
    {
        SetName(name);
    }

    void Scene::SetName(string name)
    {
        this->name = name;
    }

    string Scene::GetName()
    {
        return name;
    }

    Object* const Scene::FindFirstObjectWithTag(string tag)
    {
        for(Object* object : objects)
        {
            if(object->GetTag() == tag)
            {
                return object;
            }
        }

        Application::PrintLog(ElementCouldNotFindErr, "Couldn't found object with the tag");

        return nullptr;
    }

    vector<Object*> Scene::FindObjectsWithTag(string tag)
    {
        vector<Object*> foundObjects;

        for(Object* object : objects)
        {
            if(object->GetTag() == tag)
            {
                foundObjects.push_back(object);
            }
        }

        return foundObjects;
    }

    Object& Scene::CreateObject(string name, string tag, Layer layer)
    {
        Object* object = new Object(this, name, tag, layer);
        AddObject(object);
        return *object;
    }

    Object& Scene::CreateAndUseCameraObject()
    {
        Object& object = CreateObject("Camera", "Camera");
        Camera* camera = object.AddComponent<Camera>();
        Renderer::SetMainCamera(camera);
        return object;
    }

    void Scene::AddObject(Object* object)
    {
        if(!object)
        {
            Application::PrintLog(NullPointerErr, "Object trying to add is null");
            return;
        }

        auto obj = std::find(objects.begin(), objects.end(), object);

        if(obj != objects.end())
        {
            Application::PrintLog(NullPointerErr, "Object trying to add is already in the scene");
            return;
        }

        objects.push_back(object);
    }

    void Scene::RemoveObject(Object* object)
    {
        if(!object)
        {
            Application::PrintLog(NullPointerErr, "Object trying to remove is null");
            return;
        }

        auto obj = std::find(objects.begin(), objects.end(), object);

        if(obj == objects.end())
        {
            Application::PrintLog(ElementCouldNotFindErr, "Couldn't found object to remove in objects");
            return;
        }

        objects.erase(obj);
    }

    void Scene::ClearScene()
    {
        objects.clear();
    }

    void Scene::Awake()
    {
        BuildScene();

        for(int i = objects.size() - 1; i >= 0; --i)
        {
            Object* object = objects[i];

            if(object)
            {
                object->Awake();
            }
        }
    }

    void Scene::Start()
    {
        for(int i = objects.size() - 1; i >= 0; --i)
        {
            Object* object = objects[i];

            if(object)
            {
                object->Start();
            }
        }
    }
}

void Scene::Update()
{
    for(int i = objects.size() - 1; i >= 0; --i)
    {
        Object* object = objects[i];

        if(object)
        {
            object->Update();
        }
    }
}

void Scene::LateUpdate()
{
    for(int i = objects.size() - 1; i >= 0; --i)
    {
        Object* object = objects[i];

        if(object)
        {
            object->LateUpdate();
        }
    }

    UpdateTransforms();
}

void Scene::FixUpdate()
{
    for(int i = objects.size() - 1; i >= 0; --i)
    {
        Object* object = objects[i];

        if(object)
        {
            object->FixUpdate();
        }
    }
}

void Scene::StartOfSecond()
{
    for(int i = objects.size() - 1; i >= 0; --i)
    {
        Object* object = objects[i];

        if(object)
        {
            object->StartOfSecond();
        }
    }
}

void Scene::UpdateTransforms()
{
    for(Object* object : objects)
    {
        if(object)
        {
            object->GetTransform()->GetModelMatrix();
        }
    }
}
