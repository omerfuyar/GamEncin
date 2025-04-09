#version 460 core
layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec4 vertColor;

out vec4 color;
//TODO flat for disable color interpolation

uniform vec3  objScale, objPosition, objRotation;
uniform mat4 transformMatrix;
//TODO hard code to Shader constructor 

void main()
{
    //scale -> rotate -> translate
    vec3 scaledTransform = vertPos;

    scaledTransform.x *= objScale.x;
    scaledTransform.y *= objScale.y;
    scaledTransform.z *= objScale.z;

    vec3 rotationRad = radians(objRotation);
    mat3x3 xRotation = mat3x3(1, 0, 0, 0, cos(rotationRad.x), -sin(rotationRad.x), 0, sin(rotationRad.x), cos(rotationRad.x));
    mat3x3 yRotation = mat3x3(cos(rotationRad.y), 0, sin(rotationRad.y), 0, 1, 0, -sin(rotationRad.y), 0, cos(rotationRad.y));
    mat3x3 zRotation = mat3x3(cos(rotationRad.z), -sin(rotationRad.z), 0, sin(rotationRad.z), cos(rotationRad.z), 0, 0, 0, 1);

    vec3 rotatedTransform = scaledTransform;
    rotatedTransform = xRotation * rotatedTransform;
    rotatedTransform = yRotation * rotatedTransform;
    rotatedTransform = zRotation * rotatedTransform;

    vec4 finalPosition = vec4(rotatedTransform + objPosition, 1.0);

    finalPosition = transformMatrix * finalPosition;

    gl_Position = finalPosition;

    color = vertColor / 255.0;
}
