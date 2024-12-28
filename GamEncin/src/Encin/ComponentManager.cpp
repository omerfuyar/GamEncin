#include "ECS.h"

namespace GamEncin
{
#pragma region Transform
    
    void Transform::Translate(Vector3 amount)
    {
        position += amount;
    }

	void Transform::Rotate(Vector3 amount)
	{
		rotation += amount;
	}

	void Transform::Scale(Vector3 amount)
	{
		scale += amount;
	}

#pragma endregion
    
}
