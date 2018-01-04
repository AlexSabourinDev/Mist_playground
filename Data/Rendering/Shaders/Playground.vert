#version 430

layout (location = 0) uniform mat4 clipFromWorld;
layout (location = 1) uniform mat4 worldFromModel;

layout (location = 0) in vec3 inVert;

out vec4 interpolatorVertPos;

void main()
{
	vec4 finalPos = vec4(inVert, 1.0) * worldFromModel * clipFromWorld;
	interpolatorVertPos = vec4(inVert, 1.0);
	gl_Position = finalPos;
}