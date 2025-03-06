#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float positionDivider;
uniform vec3 scale, position, rotation;

void main()
{
    vec3 rotatedTransform = aPos;

    vec3 rotationRad = radians(rotation);

    mat3x3 xRotation = mat3x3(1, 0, 0, 0, cos(rotationRad.x), -sin(rotationRad.x), 0, sin(rotationRad.x), cos(rotationRad.x));
    mat3x3 yRotation = mat3x3(cos(rotationRad.y), 0, sin(rotationRad.y), 0, 1, 0, -sin(rotationRad.y), 0, cos(rotationRad.y));
    mat3x3 zRotation = mat3x3(cos(rotationRad.z), -sin(rotationRad.z), 0, sin(rotationRad.z), cos(rotationRad.z), 0, 0, 0, 1);

    rotatedTransform *= xRotation;
    rotatedTransform *= yRotation;
    rotatedTransform *= zRotation;

    vec3 finalPosition = vec3(rotatedTransform.x * scale.x,
                              rotatedTransform.y * scale.y,
                              rotatedTransform.z * scale.z) + position / positionDivider;

    gl_Position = vec4(finalPosition, 1.0);

    color = aColor / 255.0;
}
