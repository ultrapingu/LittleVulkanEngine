#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec3 fragPosWorld;
layout (location = 2) out vec3 fragNormalWorld;

layout (set = 0, binding = 0) uniform GlobalUbo {
	mat4 projectionMtx;
	mat4 viewMtx;
    vec4 ambientLightColor; // w is intensity
    vec4 lightPosition;
    vec4 lightColor; // w is intensity
} ubo;

layout (push_constant) uniform Push {
	mat4 modelMtx;
	mat4 normalMtx;
} push;

void main()
{
	vec4 worldPos = push.modelMtx * vec4(position, 1.0);

	gl_Position = ubo.projectionMtx * ubo.viewMtx * worldPos;

	fragColor = color;
	fragPosWorld = worldPos.xyz;
	fragNormalWorld = normalize(mat3(push.normalMtx) * normal);
}