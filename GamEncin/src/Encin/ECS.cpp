#include "ECS.h"
#include <iostream>

namespace GamEncin
{

#pragma region Entity

    Entity EntityManager::CreateEntity()
    {
        for(int i = 0; i < MaxEntities; i++)
        {
            if(!objects[i].active)
            {
                objects[i].active = true;
                return i;
            }
        }
        return -1;
    }

#pragma endregion

#pragma region Systems

    void EntityManager::DestroyEntity(Entity entity)
    {
        objects[entity].active = false;
    }
    void UpdatePsyhics()
    {
        for(int i = 0; i < MaxEntities; i++)
        {
            bodies[i].velocity += bodies[i].acceleration;
            bodies[i].velocity += bodies[i].force / bodies[i].mass;
            bodies[i].velocity -= bodies[i].velocity * bodies[i].drag;
            bodies[i].velocity -= bodies[i].velocity * bodies[i].gravityScale * MathYaman::GRAVITY;
            bodies[i].velocity -= bodies[i].velocity * bodies[i].angularDrag;
            transforms[i].position += bodies[i].velocity;
            std::cout << "Object Psyhics: " << objects[i].name << " " << bodies[i].velocity.x << "\n";
        }
    }

    void UpdateRenderers()
    {
        for(int i = 0; i < MaxEntities; i++)
        {
            //Render object
            std::cout << "Object Render: " << objects[i].name << "\n";
        }
    }

#pragma endregion
}
