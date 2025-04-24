#include "Encin/Encin.h"

namespace GamEncin
{
    Component::Component(Object* obj)
    {
        if(!obj)
        {
            Application::PrintLog(NullPointerErr, "Object trying to set to component is null");
            return;
        }

        object = obj;

        OnEnable();
    }

    Component::~Component()
    {
        OnDisable();

        if(object)
        {
            object->RemoveComponent(this);
        }
    }

    Object* const Component::GetOwnerObject()
    {
        return object;
    }

    void Component::SetObject(Object* obj)
    {
        if(!obj)
        {
            Application::PrintLog(NullPointerErr, "Object trying to set to component is null");
            return;
        }

        object = obj;
    }
}

