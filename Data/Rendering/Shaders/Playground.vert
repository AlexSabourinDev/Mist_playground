#version 430

layout (location = 0) uniform mat4 worldToClip;
layout (location = 1) uniform mat4 modelToWorld;

layout (location = 0) in vec3 inVert;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inUV;

out vec4 interpNorm;
out vec2 interpUV;

void main()
{
	vec4 finalPos = vec4(inVert, 1.0) * modelToWorld * worldToClip;
	interpNorm = vec4(inNorm, 0.0) * modelToWorld;
	interpUV = inUV;
	
	gl_Position = finalPos;
}