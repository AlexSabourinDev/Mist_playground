#version 430

layout (location = 0) uniform mat4 worldToClip;
layout (location = 1) uniform mat4 modelToWorld;

layout (location = 0) in vec3 inVert;

out vec4 interpWorldPos;
out vec4 interpLocalPos;

void main()
{
	vec4 worldPos = vec4(inVert, 1.0) * modelToWorld;
	vec4 finalPos = worldPos * worldToClip;
	interpWorldPos = worldPos;
	interpLocalPos = vec4(inVert, 1.0);
	gl_Position = finalPos;
}