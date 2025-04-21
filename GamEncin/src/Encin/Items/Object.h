#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Scene;
    class Component;
    class Transform;
    class Application;

    class Object
    {
    public:
        Object(Scene* scene = nullptr, string name = "Object", string tag = "Default", Layer layer = Layer::Default);
        ~Object();

        void SetScene(Scene* scene);
        void SetName(string name);
        void SetTag(string tag);
        void SetLayer(Layer layer);

        template <typename T>
        T* GetComponent()
        {
            for(Component* component : components)
            {
                T* castedComponent = dynamic_cast<T*>(component);

                if(castedComponent)
                {
                    return castedComponent;
                }
            }

            Application::PrintLog(NullPointerErr, "Component couldn't found in the object");
            return nullptr;
        }
        Scene* GetScene() const;
        string GetName() const;
        string GetTag() const;
        Layer GetLayer() const;
        Transform* GetTransform() const;

        template <typename T>
        T* AddComponent()
        {
            auto obj = std::find_if(components.begin(), components.end(), [](Component* component) { return dynamic_cast<T*>(component); });

            if(obj != components.end())
            {
                Application::PrintLog(ElementDuplicationErr, "Component trying to add is already in the object");
                return nullptr;
            }

            T* component = new T(this);

            if(dynamic_cast<Component*>(component))
            {
                components.push_back(component);
            }
            else
            {
                Application::Stop(TypeMismachErr, "Argument is not a Component");
            }

            return component;
        }
        void AddComponent(Component* component);
        template <typename T>
        void RemoveComponent()
        {
            auto obj = std::find_if(components.begin(), components.end(), [](Component* component) { return dynamic_cast<T*>(component); });

            if(obj != components.end())
            {
                components.erase(obj);
                delete* obj;
            }
            else
            {
                Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found component to remove");
            }
        }
        void RemoveComponent(Component* component);

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void StartOfSecond();

    private:
        Scene* scene = nullptr;

        string name = "Object",
            tag = "Default";

        Layer layer;

        Transform* transform = AddComponent<Transform>();
        vector<Component*> components;
    };
}
