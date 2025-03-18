#version 450

#extension GL_GOOGLE_include_directive : require

layout(location = 0) out vec4 finalColor;
layout(location = 1) out vec4 _;

layout(input_attachment_index = 0, set = 7, binding = 10) uniform subpassInput lightingResult;

void main()
{
    discard;
    //finalColor = subpassLoad(lightingResult);
}