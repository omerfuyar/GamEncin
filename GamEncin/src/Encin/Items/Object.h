#pragma once
#include "Encin/Tools/MathYaman.h"
#include "Encin/Tools/Toolkit.h"

namespace GamEncin
{
    class Scene;
    class Component;
    class Transform;
    class Application;
    class RigidBody;

    class Object
    {
    public:
        Object(Scene *scene = nullptr, string name = "Object", string tag = "Default", Layer layer = Layer::Default);
        ~Object();

        void SetScene(Scene *scene);
        void SetName(string name);
        void SetTag(string tag);
        void SetLayer(Layer layer);

        template <typename T>
        T *const GetComponent()
        {
            for (Component *component : components)
            {
                T *castedComponent = dynamic_cast<T *>(component);

                if (castedComponent)
                {
                    return castedComponent;
                }
            }

            Application::PrintLog(NullPointerErr, "Component couldn't found in the object");
            return nullptr;
        }
        // use it with built in typeid() function
        bool HasComponent(std::type_index componentType);
        string GetName();
        string GetTag();
        Layer GetLayer();
        Transform *const GetTransform();
        Scene *const GetScene();

        template <typename T>
        T *const AddComponent()
        {
            auto obj = std::find_if(components.begin(), components.end(), [](Component *component)
                                    { return dynamic_cast<T *>(component); });

            if (obj != components.end())
            {
                Application::PrintLog(ElementDuplicationErr, "Component trying to add is already in the object");
                return nullptr;
            }

            T *component = new T(this);

            if (dynamic_cast<Component *>(component))
            {
                components.push_back(component);
            }
            else
            {
                Application::Stop(TypeMismatchErr, "Argument is not a Component");
            }

            return component;
        }
        void AddComponent(Component *component);
        template <typename T>
        void RemoveComponent()
        {
            auto obj = std::find_if(components.begin(), components.end(), [](Component *component)
                                    { return dynamic_cast<T *>(component); });

            if (obj != components.end())
            {
                Application::PrintLog(ElementCouldNotFoundErr, "Couldn't found component to remove");
                return;
            }

            components.erase(obj);
            delete *obj;
        }
        void RemoveComponent(Component *component);
        Object &CreateChildObject();

        void OnTriggerEnter(const RigidBody *enteredRigidBody);
        void OnTriggerStay(const RigidBody *stayingRigidBody);
        void OnTriggerExit(const RigidBody *exitedRigidBody);

        void OnCollisionEnter(const RigidBody *enteredRigidBody);
        void OnCollisionStay(const RigidBody *stayingRigidBody);
        void OnCollisionExit(const RigidBody *exitedRigidBody);

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void StartOfSecond();

    private:
        Scene *scene = nullptr;

        string name = "Object",
               tag = "Default";

        Layer layer;

        Transform *transform = AddComponent<Transform>();

        vector<Component *> components;
    };
}
