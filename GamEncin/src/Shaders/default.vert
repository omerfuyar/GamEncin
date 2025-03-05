#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float positionDivider;
uniform vec3 scale, position, rotation;

void main()
{
    vec3 finalPosition = vec3(aPos.x * scale.x, aPos.y * scale.y, aPos.z * scale.z) + position / positionDivider;

    gl_Position = vec4(finalPosition, 1.0);

    color = aColor / 255;
}
