#version 460 core
out vec4 fragColor;

in vec4 color;
//TODO flat for disable color interpolation

void main()
{
    fragColor = vec4(color);
}
