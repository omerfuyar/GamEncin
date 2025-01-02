#pragma once
#include "Tools.h"

namespace GamEncin
{
    enum ComponentType
    {
        UNRECOGNIZED_TOKEN_CT, TransformCT, PsychicsBodyCT, RendererCT
    };

    class Component
    {
    public:
        ComponentType type;
        virtual ~Component() = default;

        bool isSameType(const Component& otherComponent)
        {
            return type == otherComponent.type;
        }
    };

    class UNRECOGNIZED_TOKEN : public Component
    {
    public:
        UNRECOGNIZED_TOKEN() : Component() { type = UNRECOGNIZED_TOKEN_CT; };
    };

    class Transform : public Component
    {
    public:
        Transform() : Component() { type = TransformCT; };

        Vector3
            position,
            rotation,
            scale = Vector3::One();

        // Adds to position
        void Translate(Vector3 amount);

        // Adds to rotation
        void Rotate(Vector3 amount);

        // adds to scale
        void Scale(Vector3 amount);
    };

    class PsychicsBody : public Component
    {
    public:
        PsychicsBody() : Component() { type = PsychicsBodyCT; };

        float
            mass = 1,
            drag = 0,
            angularDrag = 0,
            gravityScale = 1;
    };

    class Renderer : public Component
    {
    public:
        Renderer() : Component() { type = RendererCT; };

        vector<Vector3> vertices;
    };
}
