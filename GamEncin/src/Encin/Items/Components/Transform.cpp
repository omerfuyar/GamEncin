#include "Encin/Encin.h"

namespace GamEncin
{
    void Transform::UpdateProperties()
    {
        direction.x = CosDeg(rotation.x) * CosDeg(rotation.y);
        direction.y = SinDeg(rotation.x);
        direction.z = CosDeg(rotation.x) * SinDeg(rotation.y);
        direction.Normalize();
    }

    void Transform::Update()
    {
        UpdateProperties();
    }
}
