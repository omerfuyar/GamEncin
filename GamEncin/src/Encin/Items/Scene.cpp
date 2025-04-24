#include "Encin/Encin.h"

namespace GamEncin
{

    Object* const Scene::FindFirstObjectWitTag(string tag)
    {
        for(Object* object : objects)
        {
            if(object->GetTag() == tag)
            {
                return object;
            }
        }

        Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found object with the tag");

        return nullptr;
    }

    vector<Object*> const Scene::FindObjectsWithTag(string tag)
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
            Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found object to remove");
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
