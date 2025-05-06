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

    bool Object::HasComponent(std::type_index componentType)
    {
        for(Component* component : components)
        {
            printf("Component Type : %s\n", typeid(*component).name());
            printf("Component Type : %s\n", componentType.name());
            printf("%d\n", componentType == typeid(*component));
            if(componentType == typeid(component))
            {
                return true;
            }
        }

        return false;
    }

    string Object::GetName()
    {
        return name;
    }

    string Object::GetTag()
    {
        return tag;
    }

    Layer Object::GetLayer()
    {
        return layer;
    }

    Transform* const Object::GetTransform()
    {
        return transform;
    }

    Scene* const Object::GetScene()
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

        if(obj == components.end())
        {
            Application::PrintLog(ElementCouldNotFindErr, "Couldn't found component to remove");
            return;
        }

        components.erase(obj);
    }

    Object& Object::CreateChildObject()
    {
        Object& obj = scene->CreateObject();
        obj.GetTransform()->SetParent(transform);
        return obj;
    }

    void Object::OnTriggerEnter(RigidBody* exitedRigidBody)
    {
        for(Component* component : components)
        {
            component->OnTriggerEnter(exitedRigidBody);
        }
    }

    void Object::OnTriggerStay(RigidBody* exitedRigidBody)
    {
        for(Component* component : components)
        {
            component->OnTriggerStay(exitedRigidBody);
        }
    }

    void Object::OnTriggerExit(RigidBody* exitedRigidBody)
    {
        for(Component* component : components)
        {
            component->OnTriggerExit(exitedRigidBody);
        }
    }

    void Object::OnCollisionEnter(RigidBody* exitedRigidBody)
    {
        for(Component* component : components)
        {
            component->OnCollisionEnter(exitedRigidBody);
        }
    }

    void Object::OnCollisionStay(RigidBody* exitedRigidBody)
    {
        for(Component* component : components)
        {
            component->OnCollisionStay(exitedRigidBody);
        }
    }

    void Object::OnCollisionExit(RigidBody* exitedRigidBody)
    {
        for(Component* component : components)
        {
            component->OnCollisionExit(exitedRigidBody);
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
