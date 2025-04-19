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
        Object& CreateObject();
        Object& CreateObject(string name, string tag);
        Object& CreateAndUseCameraObject();
        Object* FindFirstObjectWitTag(string tag);
        vector<Object*> FindObjectsWithTag(string tag);

        void SetMainCamera(Camera* camera);

        void AddObject(Object* object);
        void RemoveObject(Object* object);
        void Clear();

        void Awake();
        void Start();
        void Update();
        void LateUpdate();
        void FixUpdate();
        void StartOfSecond();

    private:
        vector<Object*> objects;
    };
}
