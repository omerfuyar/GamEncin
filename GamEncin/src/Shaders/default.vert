#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float positionDivider;
uniform vec3 scale, position, rotation;

void main()
{
    vec3 rotatedTransform = aPos;

    mat3x3 xRotation = mat3x3(1, 0, 0, 0, cos(rotation.x), -sin(rotation.x), 0, sin(rotation.x), cos(rotation.x));
    mat3x3 yRotation = mat3x3(cos(rotation.y), 0, sin(rotation.y), 0, 1, 0, -sin(rotation.y), 0, cos(rotation.y));
    mat3x3 zRotation = mat3x3(cos(rotation.z), -sin(rotation.z), 0, sin(rotation.z), cos(rotation.z), 0, 0, 0, 1);

    rotatedTransform = xRotation * rotatedTransform;
    rotatedTransform = yRotation * rotatedTransform;
    rotatedTransform = zRotation * rotatedTransform;

    vec3 finalPosition = vec3(rotatedTransform.x * scale.x,
                              rotatedTransform.y * scale.y,
                              rotatedTransform.z * scale.z) + position / positionDivider;

    gl_Position = vec4(finalPosition, 1.0);

    color = aColor / 255.0;
}
