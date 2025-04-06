#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

#include <algorithm>

namespace GamEncin
{
    class Scene;
    class Component;
    class Transform;
    class Application;

    class Object
    {
    public:
        Object() = default;
        Object(Scene* scene);
        Object(Scene* scene, string name, string tag);
        Object(Scene* scene, string name, string tag, Layer layer);
        ~Object();

        string name = "Object",
            tag = "Default Tag";

        Layer layer;

        Scene* scene = nullptr;
        vector<Component*> components;
        Transform* transform = AddComponent<Transform>();

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
            Component* castedComponent = dynamic_cast<Component*>(component);

            if(castedComponent)
            {
                components.push_back(component);
            }
            else
            {
                Application::Stop(TypeMismachErr, "Argument is not a Component");
            }

            return component;
        }

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
    };
}
