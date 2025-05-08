#include "Encin/Encin.h"

namespace GamEncin
{

    Object::Object(Scene* scene, string name, string tag, Layer layer)
    {
        this->scene = scene;
        this->name = name;
        this->tag = tag;
        this->layer = layer;

        this->transform = AddComponent<Transform>();
    }

    Object::~Object()
    {
        printf("Object destructor called for %p, name %s\n", this, name.c_str()); // Debug print

        Transform* tr = GetTransform()->GetParent();
        if(tr)
        {
            tr->RemoveChild(this->GetTransform());
        }

        if(GetScene())
        {
            GetScene()->RemoveObject(this);
        }

        for(Component* component : components)
        {
            if(!component)
            {
                Application::PrintLog(NullPointerErr, "Component trying to delete is null");
                continue;
            }

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

    void Object::OnTriggerEnter(RigidBody* enteredRigidBody)
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->OnTriggerEnter(enteredRigidBody);
        }
    }

    void Object::OnTriggerStay(RigidBody* stayingRigidBody)
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->OnTriggerStay(stayingRigidBody);
        }
    }

    void Object::OnTriggerExit(RigidBody* exitedRigidBody)
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->OnTriggerExit(exitedRigidBody);
        }
    }

    void Object::OnCollisionEnter(RigidBody* enteredRigidBody)
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->OnCollisionEnter(enteredRigidBody);
        }
    }

    void Object::OnCollisionStay(RigidBody* stayingRigidBody)
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->OnCollisionStay(stayingRigidBody);
        }
    }

    void Object::OnCollisionExit(RigidBody* exitedRigidBody)
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->OnCollisionExit(exitedRigidBody);
        }
    }

    void Object::Awake()
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->Awake();
        }
    }

    void Object::Start()
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->Start();
        }
    }

    void Object::Update()
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->Update();
        }
    }

    void Object::LateUpdate()
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->LateUpdate();
        }
    }

    void Object::FixUpdate()
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->FixUpdate();
        }
    }

    void Object::StartOfSecond()
    {
        for(int i = components.size() - 1; i >= 0; i--)
        {
            Component* component = components[i];
            component->StartOfSecond();
        }
    }
}
