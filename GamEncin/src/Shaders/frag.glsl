#version 460 core
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

flat in uint64_t modelTextureHandle;
in vec4 fragColor;
in vec3 fragNormal;
in vec2 fragUV;

out vec4 outColor;

void main()
{
    sampler2D albedoTexture = sampler2D(uvec2(modelTextureHandle));
    vec4 texColor = texture(albedoTexture, fragUV);
    outColor = texColor * fragColor;
}
