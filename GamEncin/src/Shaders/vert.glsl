#version 460 core

layout(std430, binding = 0) buffer ModelWorldMatrices
{
    mat4 modelWorldMatrices[]; // variable length array
};
    
layout (location = 0) in int vertObjectId;
layout (location = 1) in vec3 vertPos;
layout (location = 2) in vec4 vertColor;

out vec4 color;
//TODO flat for disable color interpolation

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
//TODO hard code to Shader constructor 

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelWorldMatrices[vertObjectId] * vec4(vertPos, 1.0);
    //vUV = aUV;
    //vNormal = mat3(transpose(inverse(uModel))) * aNormal; // for correct lighting

    color = vertColor / 255.0;
}
