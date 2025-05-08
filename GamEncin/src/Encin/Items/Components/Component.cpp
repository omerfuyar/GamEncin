#include "Encin/Encin.h"

namespace GamEncin
{
    Component::Component(Object *obj)
    {
        if (!obj)
        {
            Application::PrintLog(NullPointerErr, "Object trying to set to component is null");
            return;
        }

        object = obj;

        OnEnable();
    }

    Component::~Component()
    {
        printf("Component destructor called for %p, name %s\n", this, object->GetName().c_str()); // Debug print

        OnDisable();
    }

    Object *const Component::GetOwnerObject()
    {
        return object;
    }

    void Component::SetObject(Object *obj)
    {
        if (!obj)
        {
            Application::PrintLog(NullPointerErr, "Object trying to set to component is null");
            return;
        }

        object = obj;
    }
}
