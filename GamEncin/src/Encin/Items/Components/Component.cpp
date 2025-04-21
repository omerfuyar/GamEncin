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
    }

    Component::~Component()
    {
        if(object)
        {
            object->RemoveComponent(this);
        }
    }

    Object* Component::GetOwnerObject() const
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
