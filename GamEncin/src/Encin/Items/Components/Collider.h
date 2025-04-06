#pragma once
#include "Encin/Items/Components/Component.h"
#include "Encin/Tools/MathYaman.h"

namespace GamEncin
{
    class Collider : public Component
    {
        Collider(Object* object) : Component(object) {};
    };
}
