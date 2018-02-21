#version 430

layout (location = 0) uniform mat4 worldToClip;

layout (location = 0) in vec3 inVert;
layout (location = 1) in vec3 inNorm;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 modelToWorldPos;
layout (location = 4) in vec4 modelToWorldRot;

out vec3 interpNorm;
out vec2 interpUV;

vec3 QuatMul(vec4 lhs, vec3 rhs)
{
	vec3 t = 2.0 * cross(lhs.xyz, rhs);
	return rhs + lhs.w * t + cross(lhs.xyz, t);
}

void main()
{
	vec3 worldPos = QuatMul(modelToWorldRot, inVert) + modelToWorldPos;
	
	vec4 finalPos = vec4(worldPos, 1.0) * worldToClip;
	interpNorm = QuatMul(modelToWorldRot, inNorm);
	interpUV = inUV;
	
	gl_Position = finalPos;
}