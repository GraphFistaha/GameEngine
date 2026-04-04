#version 450

layout(location = 0) in vec2 uv;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2DArray u_sampler;

void main() {
    fragColor = vec4(texture(u_sampler, vec3(uv, 0)).rgb,  1.0);
}