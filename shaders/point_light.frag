#version 450

layout (location = 0) in vec2 fragOffset;
layout (location = 0) out vec4 outColor;

layout (set = 0, binding = 0) uniform GlobalUbo {
	mat4 projectionMtx;
	mat4 viewMtx;
    vec4 ambientLightColor; // w is intensity
    vec4 lightPosition;
    vec4 lightColor; // w is intensity
} ubo;

void main()
{
    float dist = sqrt(dot(fragOffset, fragOffset));

    if (dist >= 1.0f)
    {
        discard;
    }

    outColor = vec4(ubo.lightColor.xyz, 1.0f);
}