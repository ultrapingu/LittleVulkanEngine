#version 450

const vec2 OFFSETS[6] = vec2[](
  vec2(-1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, -1.0),
  vec2(1.0, -1.0),
  vec2(-1.0, 1.0),
  vec2(1.0, 1.0)
);

layout (location = 0) out vec2 fragOffset;

layout (set = 0, binding = 0) uniform GlobalUbo {
	mat4 projection;
	mat4 view;
    vec4 ambientLightColor; // w is intensity
    vec4 lightPosition;
    vec4 lightColor; // w is intensity
} ubo;

const float LIGHT_RADIUS = 0.02f;

void main()
{
    fragOffset = OFFSETS[gl_VertexIndex];
    vec4 lightInCameraSpace = ubo.view * vec4(ubo.lightPosition.xyz, 1.0f);
    vec4 positionInCameraSpace = lightInCameraSpace + LIGHT_RADIUS * vec4(fragOffset, 0.0f, 0.0f);    
    gl_Position = ubo.projection * positionInCameraSpace;
}