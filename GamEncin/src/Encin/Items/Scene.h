#pragma once
#include "Encin/Tools/MathYaman.h"

namespace GamEncin
{
    class Object;
    class Renderer;
    class Camera;

    class Scene
    {
    public:
        template<typename T>
        vector<T*> FindComponentsByType()
        {
            vector<T*> result;

            for(Object* object : objects)
            {
                T* component = object->GetComponent<T>();
                if(component)
                {
                    result.push_back(component);
                }
            }

            return result;
        }
        Object* const FindFirstObjectWithTag(string tag);
        vector<Object* > FindObjectsWithTag(string tag);

        Object& CreateObject(string name = "Object", string tag = "Default", Layer layer = Layer::Default);
        Object& CreateAndUseCameraObject();
        void AddObject(Object* object);
        void RemoveObject(Object* object);
        void ClearScene();

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void StartOfSecond();

    private:
        vector<Object*> objects;

        void UpdateTransforms();
    };
}
