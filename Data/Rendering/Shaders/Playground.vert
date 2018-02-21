#version 430

layout (location = 0) uniform mat4 worldToClip;

layout (location = 0) in vec3 inVert;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec4 modelToWorld1;
layout (location = 4) in vec4 modelToWorld2;
layout (location = 5) in vec4 modelToWorld3;
layout (location = 6) in vec4 modelToWorld4;

out vec4 interpNorm;
out vec2 interpUV;

void main()
{
	mat4 modelToWorld = mat4(modelToWorld1, modelToWorld2, modelToWorld3, modelToWorld4);

	vec4 finalPos = vec4(inVert, 1.0) * modelToWorld * worldToClip;
	interpNorm = vec4(inNorm, 0.0) * modelToWorld;
	interpUV = inUV;
	
	gl_Position = finalPos;
}