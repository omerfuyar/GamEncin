#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

layout(std430, binding = 0) buffer ModelWorldMatrices
{
    mat4 modelWorldMatrices[];
};

layout(std430, binding = 1) buffer ModelTextureHandles
{
    uint64_t modelTextureHandles[]; 
};

layout (location = 0) in uint vertObjectId;
layout (location = 1) in vec3 vertPosition;
layout (location = 2) in vec3 vertNormal;
layout (location = 3) in vec2 vertUV;

flat out uint64_t modelTextureHandle;
out mat4 modelMatrix; //dunno why Im sending this to fragment shader but it may be useful.
out vec3 fragNormal;
out vec2 fragUV;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    modelMatrix = modelWorldMatrices[vertObjectId];
    modelTextureHandle = modelTextureHandles[vertObjectId];

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertPosition, 1.0);

    fragNormal = mat3(transpose(inverse(modelMatrix))) * vertNormal;

    fragUV = vertUV;
}
