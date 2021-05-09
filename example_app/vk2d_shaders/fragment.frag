#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in flat float fragTexIndex;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler batch_sampler;
layout(set = 0, binding = 1) uniform texture2D textures[32];

void main() 
{
    outColor = texture(sampler2D(textures[int(fragTexIndex)], batch_sampler), fragTexCoord) * fragColor;
}