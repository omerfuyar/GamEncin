#include "Encin/Encin.h"

namespace GamEncin
{

    Object::Object(Scene* scene, string name, string tag, Layer layer)
    {
        this->scene = scene;
        this->name = name;
        this->tag = tag;
        this->layer = layer;
    }

    Object::~Object()
    {
        for(Component* component : components)
        {
            delete component;
        }

        components.clear();
    }

    void Object::SetScene(Scene* scene)
    {
        this->scene = scene;
    }

    void Object::SetName(string name)
    {
        this->name = name;
    }

    void Object::SetTag(string tag)
    {
        this->tag = tag;
    }

    void Object::SetLayer(Layer layer)
    {
        this->layer = layer;
    }

    string Object::GetName() const
    {
        return name;
    }

    string Object::GetTag() const
    {
        return tag;
    }

    Layer Object::GetLayer() const
    {
        return layer;
    }

    Transform* Object::GetTransform() const
    {
        return transform;
    }

    Scene* Object::GetScene() const
    {
        return scene;
    }

    void Object::AddComponent(Component* component)
    {
        if(!component)
        {
            Application::PrintLog(NullPointerErr, "Component trying to add is null");
            return;
        }

        auto obj = std::find(components.begin(), components.end(), component);

        if(obj != components.end())
        {
            Application::PrintLog(ElementDuplicationErr, "Component trying to add is already in the object");
            return;
        }

        components.push_back(component);
    }

    void Object::RemoveComponent(Component* component)
    {
        if(!component)
        {
            Application::PrintLog(NullPointerErr, "Component trying to remove is null");
            return;
        }

        auto obj = std::find(components.begin(), components.end(), component);

        if(obj != components.end())
        {
            components.erase(obj);
        }
        else
        {
            Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found component to remove");
        }
    }

    void Object::Awake()
    {
        for(Component* component : components)
        {
            component->Awake();
        }
    }

    void Object::Start()
    {
        for(Component* component : components)
        {
            component->Start();
        }
    }

    void Object::Update()
    {
        for(Component* component : components)
        {
            component->Update();
        }
    }

    void Object::LateUpdate()
    {
        for(Component* component : components)
        {
            component->LateUpdate();
        }
    }

    void Object::FixUpdate()
    {
        for(Component* component : components)
        {
            component->FixUpdate();
        }
    }

    void Object::StartOfSecond()
    {
        for(Component* component : components)
        {
            component->StartOfSecond();
        }
    }
}
