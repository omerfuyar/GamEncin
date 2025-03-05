#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform vec3 scale;

void main()
{
    gl_Position = vec4(aPos.x * scale.x, aPos.y * scale.y, aPos.z * scale.z, 1.0);
    color = aColor;
}
