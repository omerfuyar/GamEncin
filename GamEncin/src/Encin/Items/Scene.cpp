#include "Encin/Encin.h"

namespace GamEncin
{
    Object& Scene::CreateObject()
    {
        Object* object = new Object(this);
        AddObject(object);
        return *object;
    }

    Object& Scene::CreateObject(string name, string tag)
    {
        Object* object = new Object(this, name, tag);
        AddObject(object);
        return *object;
    }

    Object& Scene::CreateAndUseCameraObject()
    {
        Object& object = CreateObject("Camera", "Camera");
        Camera* camera = object.AddComponent<Camera>();
        SetMainCamera(camera);
        return object;
    }

    Object* Scene::FindFirstObjectWitTag(string tag)
    {
        for(Object* object : objects)
        {
            if(object->tag == tag)
            {
                return object;
            }
        }

        Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found object with the tag");

        return nullptr;
    }

    vector<Object*> Scene::FindObjectsWithTag(string tag)
    {
        vector<Object*> foundObjects;

        for(Object* object : objects)
        {
            if(object->tag == tag)
            {
                foundObjects.push_back(object);
            }
        }

        return foundObjects;
    }

    void Scene::SetMainCamera(Camera* camera)
    {
        if(!camera)
        {
            Application::PrintLog(NullPointerErr, "Camera trying to set is null");
            return;
        }

        Renderer::SetMainCamera(camera);
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

        if(obj != objects.end())
        {
            objects.erase(obj);
        }
        else
        {
            Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found object to remove");
        }
    }

    void Scene::Clear()
    {
        objects.clear();
    }

    void Scene::Awake()
    {
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

    void Scene::LateUpdate()
    {
        for(Object* object : objects)
        {
            object->LateUpdate();
        }
    }

    void Scene::FixUpdate()
    {
        for(Object* object : objects)
        {
            object->FixUpdate();
        }
    }

    void Scene::StartOfSecond()
    {
        for(Object* object : objects)
        {
            object->StartOfSecond();
        }
    }
}
