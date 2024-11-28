#version 450

layout(location = 0) in vec4 color;
layout(location = 1) in vec2 texCoord;

layout(binding = 1) uniform sampler2D g_texture;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(g_texture, texCoord);
}