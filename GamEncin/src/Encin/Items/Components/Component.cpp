#include "Encin/Encin.h"

namespace GamEncin
{

    Component::Component(Object* object)
    {
        this->object = object;
    }

    Component::~Component()
    {
        if(object)
        {
            object->RemoveComponent(this);
        }
    }
}
