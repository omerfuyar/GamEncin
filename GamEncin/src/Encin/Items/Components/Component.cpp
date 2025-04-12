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
        else
        {
            object = obj;
        }
    }

    Component::~Component()
    {
        if(object)
        {
            object->RemoveComponent(this);
        }
    }
}
