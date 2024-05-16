#version 450 core

layout (location = 0) in vec2 vtxUv;
layout (location = 1) in vec4 vtxColor;

layout (std140, binding = 0) uniform VertUBO {
    mat4 proj;
    int isUseSrgb;
} ubo;

layout (binding = 1) uniform sampler2D tex;

layout (location = 0) out vec4 outColor;

// Converts a color from sRGB gamma to linear light gamma
vec4 toLinear(vec4 sRGB)
{
    bvec3 cutoff = lessThan(sRGB.rgb, vec3(0.04045));
    vec3 higher = pow((sRGB.rgb + vec3(0.055))/vec3(1.055), vec3(2.4));
    vec3 lower = sRGB.rgb/vec3(12.92);

    return vec4(mix(higher, lower, cutoff), sRGB.a);
}

void main() {
    if (ubo.isUseSrgb == 1) {
        outColor = vtxColor * texture(tex, vtxUv);
    } else {
        outColor = toLinear(vtxColor) * texture(tex, vtxUv);
    }
}