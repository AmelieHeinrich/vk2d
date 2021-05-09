#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform UBO {
    mat4 view;
    mat4 projection;
} ubo;  

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out flat float fragTexIndex;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in int inTexIndex;

void main() {
    gl_Position =  ubo.projection * ubo.view * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexCoord = inTexCoord;
    fragTexIndex = inTexIndex;
}