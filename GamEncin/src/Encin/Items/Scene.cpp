#include "Encin/Encin.h"

namespace GamEncin
{

    Scene::Scene()
    {
        renderer = new Renderer();
    }

    Object* Scene::CreateObject()
    {
        Object* object = new Object(this);
        AddObject(object);
        return object;
    }

    Object* Scene::CreateObject(string name, string tag)
    {
        Object* object = new Object(this, name, tag);
        AddObject(object);
        return object;
    }

    Object* Scene::CreateAndUseCameraObject(Vector2Int size)
    {
        Object* object = CreateObject("Camera", "Camera");
        object->transform->position = Vector3(0, 0, 0);
        object->transform->rotation = Vector3(0, -90, 0);
        Camera* camera = object->AddComponent<Camera>();
        SetMainCamera(camera);
        camera->size = size;
        return object;
    }

    void Scene::SetMainCamera(Camera* camera)
    {
        if(!camera)
        {
            Application::PrintLog(NullPointerErr, "Camera trying to set is null");
            return;
        }

        renderer->mainCamera = camera;
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
        renderer->InitialRender();

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

        renderer->RenderFrame();
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

        renderer->RenderFrame();
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
